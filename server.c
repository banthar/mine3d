
#include "config.h"

#include "socket.h"

#include "math.h"
#include "world.h"
#include "worldgen.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <zlib.h>
#include <setjmp.h>
#include "SDL.h"

typedef struct Client Client;

struct Client
{
    int state;
    Socket socket;
    SDL_mutex* mutex;
    SDL_Thread* thread;
    World* world;
    int eid;
};

static void sendLoginRequest(Socket* socket)
{
    writeByte(socket,0x01);
    writeInt(socket,0);
    writeString16(socket,"");
    writeLong(socket,0);
    writeByte(socket,0);
}

static void sendHandshake(Socket* socket)
{
    writeByte(socket,0x02);
    writeString16(socket,"-");
}

private void sendPlayerPositionAndLook(Socket* socket)
{

    float z=90;

    writeByte(socket,0x0d);
    writeDouble(socket,0);
    writeDouble(socket,z+1.62);
    writeDouble(socket,z);
    writeDouble(socket,0);
    writeFloat(socket,0);
    writeFloat(socket,0);
    writeBool(socket,true);

}

private void sendPreChunk(Socket* socket, int x, int y, bool mode)
{
    writeByte(socket,0x32);
    writeInt(socket,x);
    writeInt(socket,y);
    writeBool(socket,mode);
}

static void setNibble(byte* ptr, int n, byte val)
{
    ptr[n/2]&=n&1?0x0f:0xf0;
    ptr[n/2]|=n&1?val<<4:val;
}

private void sendMapChunk(Client* client, Vec4i pos, Vec4i size)
{

    writeByte(&client->socket,0x33);

    writeInt(&client->socket,pos[0]);
    writeShort(&client->socket,pos[2]);
    writeInt(&client->socket,pos[1]);

    writeByte(&client->socket,size[0]-1);
    writeByte(&client->socket,size[2]-1);
    writeByte(&client->socket,size[1]-1);

    int area=size[0]*size[1]*size[2];

    size_t uncompressed_size=area*5/2;
    byte uncompressed[uncompressed_size];

    byte* ids=uncompressed;
    byte* metadata=ids+area;
    byte* light=metadata+area/2;
    byte* skyLight=light+area/2;

    assert((skyLight+area/2-uncompressed)==uncompressed_size);

    for(int x=0;x<size[0];x++)
    for(int y=0;y<size[1];y++)
    for(int z=0;z<size[2];z++)
    {
        int i=z+size[2]*(y+size[1]*x);
        assert(ids+i<metadata);
        Block b=worldGet(client->world,pos+(Vec4i){x,y,z});

        setNibble(metadata,i,b.metadata);
        setNibble(light,i,b.light);
        setNibble(skyLight,i,b.skyLight);
        ids[i]=b.id;
    }


    size_t compressed_size=uncompressed_size;
    byte compressed[compressed_size];

    if(compress(compressed,&compressed_size,uncompressed,sizeof(uncompressed)) != Z_OK)
    {
        panic("uncompress error");
    }

    writeInt(&client->socket,compressed_size);
    socketWrite(&client->socket,compressed,compressed_size);

}

static void sendWorld(Client* client)
{

    int size=8;

    for(int x=-size;x<size;x++)
    for(int y=-size;y<size;y++)
    {

        for(int z=0;z<8;z++)
            prepareSegment(client->world,(Vec4i){x,y,z});

        sendPreChunk(&client->socket,x,y,true);
        sendMapChunk(client, (Vec4i){x*16,y*16,0}, (Vec4i){16,16,128});

    }

}

static void sendKick(Socket* socket, char* reason)
{
    writeByte(socket,0xff);
    writeString16(socket,reason);
}

typedef void PacketHandler(Socket* socket);

static void readKeepAlive(Socket* socket)
{
}

static void readLoginRequest(Socket* socket)
{
    readInt(socket);
    char* name=readString16(socket);
    printf("login: %s\n",name);
    free(name);
    readLong(socket);
    readByte(socket);
}

static void readHandshake(Socket* socket)
{
    char* name=readString16(socket);
    printf("handshake: %s\n",name);
    free(name);
}

static void readPlayerFlying(Socket* socket)
{
    /*bool on_ground=*/readBool(socket);
}

static void readPlayerPosition(Socket* socket)
{
    /*double x=*/readDouble(socket);
    /*double stance=*/readDouble(socket);
    /*double y=*/readDouble(socket);
    /*double z=*/readDouble(socket);
    /*bool on_ground=*/readBool(socket);
}

static void readPlayerLook(Socket* socket)
{
    /*float yaw=*/readFloat(socket);
    /*float pitch=*/readFloat(socket);
    /*bool on_ground=*/readBool(socket);
}

static void readPlayerPositionAndLook(Socket* socket)
{
    /*double x=*/readDouble(socket);
    /*double stance=*/readDouble(socket);
    /*double y=*/readDouble(socket);
    /*double z=*/readDouble(socket);
    /*float yaw=*/readFloat(socket);
    /*float pitch=*/readFloat(socket);
    /*bool on_ground=*/readBool(socket);
}

uint32_t lastPacket;

static void readPlayerDigging(Socket* socket)
{
    int state=readByte(socket);
    int x=readInt(socket);
    int z=readByte(socket);
    int y=readInt(socket);
    int face=readByte(socket);

    printf("dig: t:+%u s:%i x:%i y:%i z:%i f:%i\n",SDL_GetTicks()-lastPacket,state,x,y,z,face);

    lastPacket=SDL_GetTicks();

}

static void readAnimation(Socket* socket)
{
    int eid=readInt(socket);
    int animation=readByte(socket);

    printf("ani: t:+%u %i %i\n",SDL_GetTicks()-lastPacket,eid,animation);

    lastPacket=SDL_GetTicks();

}

PacketHandler* packetHandler[256]={
    [0x00] = readKeepAlive,

    [0x0A] = readPlayerFlying,
    [0x0B] = readPlayerPosition,
    [0x0C] = readPlayerLook,
    [0x0D] = readPlayerPositionAndLook,
    [0x0E] = readPlayerDigging,
    [0x12] = readAnimation,
};

static int clientPingThread(void* data)
{
    Client* client=(Client*)data;

    while(client->state==1)
    {
        SDL_LockMutex(client->mutex);
        writeByte(&client->socket,0x00);
        SDL_UnlockMutex(client->mutex);

        SDL_Delay(1000);

    }

    return 0;

}

static int clientThread(void* data)
{

    Client* client=(Client*)data;
    jmp_buf env;

    void onError()
    {
        fprintf(stderr,"client error\n");
        socketFlush(&client->socket);
        longjmp(env,1);
    }


    client->socket.onError=onError;
    client->mutex=SDL_CreateMutex();

    SDL_LockMutex(client->mutex);

    if(!setjmp(env))
    {

        readByte(&client->socket)==0x02 or panic("expected handshake");
        readHandshake(&client->socket);
        sendHandshake(&client->socket);
        socketFlush(&client->socket);
        readByte(&client->socket)==0x01 or panic("expected login request");
        readLoginRequest(&client->socket);
        sendLoginRequest(&client->socket);
        sendPlayerPositionAndLook(&client->socket);
        sendWorld(client);
        socketFlush(&client->socket);

        SDL_CreateThread(clientPingThread,client);


        while(true)
        {

            socketFlush(&client->socket);
            SDL_UnlockMutex(client->mutex);
            byte packet_id=readByte(&client->socket);
            SDL_LockMutex(client->mutex);

            PacketHandler* handler=packetHandler[packet_id];

            if(handler==NULL)
            {
                char buf[1024];
                sprintf(buf,"Unknown packet: 0x%02x",packet_id);
                fprintf(stderr,buf);
                sendKick(&client->socket,buf);
                socketFlush(&client->socket);
                break;
            }
            else
            {
                (*handler)(&client->socket);
            }


        }

    }

    close(client->socket.fd);

    SDL_UnlockMutex(client->mutex);
    SDL_DestroyMutex(client->mutex);

    client->state=0;

    return 0;

}

static int openListenSock()
{
    int listen_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    assert(listen_sock>0);

    int t=1;
    setsockopt (listen_sock, SOL_SOCKET, SO_REUSEADDR, &t, sizeof (t));

    struct sockaddr_in stSockAddr={0};

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(25565);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;

    if( bind(listen_sock,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)) != 0 )
        panic("bind error");

    if( listen(listen_sock, 10) != 0 )
        panic("listen error");

    return listen_sock;

}

int main(int argc, char* argv[])
{

    socketInit();

    int socket=openListenSock();

    assert(socket>=0);

    World world;
    worldInit(&world);

    Client client[16]={};

    while(true)
    {

        int firstFree=-1;

        for(int i=lengthof(client)-1;i>=0;i--)
        {
            if(client[i].state==0)
            {
                firstFree=i;

                if(client[i].thread!=NULL)
                {
                    SDL_WaitThread(client[i].thread,NULL);
                    client[i]=(Client){0};
                }
            }
        }

        int fd=accept(socket,NULL,NULL);

        assert(fd>=0);

        if(firstFree>=0)
        {
            client[firstFree].state=1;
            client[firstFree].world=&world;
            client[firstFree].socket.fd=fd;
            client[firstFree].thread=SDL_CreateThread(clientThread,&client[firstFree]);
        }
        else
        {
            close(fd);
        }

    }

}
