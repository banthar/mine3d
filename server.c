
#include "config.h"

#include "socket.h"

#include "math.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <zlib.h>
#include "SDL.h"
#include "SDL_image.h"

static SDL_Surface* animation;

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

	float z=64;

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
	ptr[n/2]|=n&1?val<<4:val;
}

private void sendMapChunk(Socket* socket, Vec4i pos, Vec4i size, uint16_t f(int,int,int))
{

	writeByte(socket,0x33);

	writeInt(socket,pos[0]);
	writeShort(socket,pos[2]);
	writeInt(socket,pos[1]);
	
	writeByte(socket,size[0]-1);
	writeByte(socket,size[2]-1);
	writeByte(socket,size[1]-1);

	int area=size[0]*size[1]*size[2];

	size_t uncompressed_size=area*5/2;
	byte uncompressed[uncompressed_size];

	byte* ids=uncompressed;
	byte* metadata=ids+area;
	byte* light=metadata+area/2;
	byte* skyLight=light+area/2;

	assert((skyLight+area/2-uncompressed)==uncompressed_size);

	memset(metadata,0x0,area/2);
	memset(light,0x0,area/2);
	memset(skyLight,0xff,area/2);

	for(int x=0;x<size[0];x++)
	for(int y=0;y<size[1];y++)
	for(int z=0;z<size[2];z++)
	{
		int pos=z+size[2]*(x+size[1]*y);
		assert(ids+pos<metadata);
		uint16_t b=f(x,y,z);
		setNibble(metadata,pos,b>>8);
		ids[pos]=b&0xff;
	}


	size_t compressed_size=uncompressed_size;
	byte compressed[compressed_size];

	if(compress(compressed,&compressed_size,uncompressed,sizeof(uncompressed)) != Z_OK)
	{
		panic("uncompress error");
	}
	
	writeInt(socket,compressed_size);
	socketWrite(socket,compressed,compressed_size);

}

static void sendWorld(Socket* socket)
{

	uint16_t f(int x,int y, int z)
	{
		return (z<63)?35:0;
	}

	int size=2;

	for(int x=-size;x<size;x++)
	for(int y=-size;y<size;y++)
	{

		sendPreChunk(socket,x,y,true);
		sendMapChunk(socket, (Vec4i){x*16,y*16,0}, (Vec4i){16,16,128},&f);

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
	readLong(socket);
	readByte(socket);
	printf("login: %s\n",name);
}

static void readHandshake(Socket* socket)
{
	char* name=readString16(socket);
	printf("handshake: %s\n",name);
}

static void readPlayerFlying(Socket* socket)
{
	bool on_ground=readBool(socket);
}

static void readPlayerPosition(Socket* socket)
{
	double x=readDouble(socket);
	double stance=readDouble(socket);
	double y=readDouble(socket);
	double z=readDouble(socket);
	bool on_ground=readBool(socket);
}

static void readPlayerLook(Socket* socket)
{
	float yaw=readFloat(socket);
	float pitch=readFloat(socket);
	bool on_ground=readBool(socket);
}

static void readPlayerPositionAndLook(Socket* socket)
{
	double x=readDouble(socket);
	double stance=readDouble(socket);
	double y=readDouble(socket);
	double z=readDouble(socket);
	float yaw=readFloat(socket);
	float pitch=readFloat(socket);
	bool on_ground=readBool(socket);
}

static void readPlayerDigging(Socket* socket)
{
	readByte(socket);
	readInt(socket);
	readByte(socket);
	readInt(socket);
	readByte(socket);
}

static void readAnimation(Socket* socket)
{
	readInt(socket);
	readByte(socket);
}


PacketHandler* packetHandler[256]={
	[0x00] = readKeepAlive,
//	[0x01] = readLoginRequest,
//	[0x02] = readHandshake,
	[0x0A] = readPlayerFlying,
	[0x0B] = readPlayerPosition,
	[0x0C] = readPlayerLook,
	[0x0D] = readPlayerPositionAndLook,
	[0x0E] = readPlayerDigging,
	[0x12] = readAnimation,
};

typedef struct
{
	Socket socket;
	SDL_mutex* mutex;
}Client;

static byte getPixel(SDL_Surface* img, int x,int y)
{
	return ((byte*)img->pixels)[x+y*img->pitch];
}

int animationWidth=16;
int animationHeight=16;

static int animationThread(void* data)
{

	int frame=0;

	int pitch=animation->w/animationWidth;
	int length=animation->w*animation->h/animationHeight/animationWidth;


	printf("%i %i %i\n",frame,pitch,length);

	Client* client=data;

	uint16_t f(int x, int y, int z)
	{
		return (getPixel(animation,(frame%pitch)*animationWidth+x,(frame/pitch)*animationHeight+animationHeight-1-z)<<8)|35;
	}

	while(true)
	{

		frame=(frame+1)%length;


		SDL_LockMutex(client->mutex);
		sendMapChunk(&client->socket, (Vec4i){-16,-16,63}, (Vec4i){16,1,16},&f);
		SDL_UnlockMutex(client->mutex);
		SDL_Delay(1000/25);
	}

	return 0;

}

static int clientThread(void* data)
{

	socketInit();
	Client client={
		.socket={
			.fd=(intptr_t)data,
		},
		.mutex=SDL_CreateMutex(),
	};

	SDL_LockMutex(client.mutex);

	assert(readByte(&client.socket)==0x02);
	readHandshake(&client.socket);
	sendHandshake(&client.socket);
	socketFlush(&client.socket);
	assert(readByte(&client.socket)==0x01);
	readLoginRequest(&client.socket);
	sendLoginRequest(&client.socket);
	sendPlayerPositionAndLook(&client.socket);
	sendWorld(&client.socket);
	socketFlush(&client.socket);

	SDL_CreateThread(animationThread,&client);

	while(true)
	{

		socketFlush(&client.socket);
		SDL_UnlockMutex(client.mutex);
		byte packet_id=readByte(&client.socket);
		SDL_LockMutex(client.mutex);

		PacketHandler* handler=packetHandler[packet_id];

		if(handler==NULL)
		{
			char buf[1024];
			sprintf(buf,"Unknown packet: 0x%02x",packet_id);
			sendKick(&client.socket,buf);
			socketFlush(&client.socket);
			break;
		}
		else
		{
			(*handler)(&client.socket);
		}


	}

	socketFlush(&client.socket);
	SDL_Delay(25);
	close(client.socket.fd);

	SDL_UnlockMutex(client.mutex);

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

	assert(! bind(listen_sock,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)));

	assert(! listen(listen_sock, 10));

	return listen_sock;

}

int main(int argc, char* argv[])
{

	animation=IMG_Load("rick.png");

	assert(animation!=NULL);

	int socket=openListenSock();

	assert(socket>=0);

	while(true)
	{

		int client=accept(socket,NULL,NULL);

		assert(client>=0);

		SDL_CreateThread(clientThread,(void*)(intptr_t)client);

	}
}
