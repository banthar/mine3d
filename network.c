
#include "config.h"

#include "network.h"

#include "client.h"
#include "socket.h"
#include "world.h"
#include "math.h"

#include <math.h>
#include "SDL.h"
#include "SDL_net.h"
#include <iconv.h>
#include <zlib.h>

#define or ?:

#define PACKET_DEBUG_START(id,name) {}

static char* playerName="banthar1";

static int stage=0;

private void sendLoginRequest(Client* client)
{

    SDL_LockMutex(client->socketLock);

    writeByte(&client->socket,0x01);
    writeInt(&client->socket,14);
    writeString16(&client->socket,playerName);
    writeLong(&client->socket,0llu);
    writeByte(&client->socket,0);

    SDL_UnlockMutex(client->socketLock);

}

private void sendHandShake(Client* client)
{

    SDL_LockMutex(client->socketLock);

    writeByte(&client->socket,0x02);
    writeString16(&client->socket,playerName);

    SDL_UnlockMutex(client->socketLock);

}

private void sendPlayerPositionAndLook(Client* client)
{

    SDL_LockMutex(client->socketLock);

    writeByte(&client->socket,0x0d);
    writeDouble(&client->socket,client->world.player.pos[0]-client->world.player.headOffset[0]);
    writeDouble(&client->socket,client->world.player.pos[2]-client->world.player.headOffset[2]);
    writeDouble(&client->socket,client->world.player.pos[2]-client->world.player.headOffset[2]+1.62);
    writeDouble(&client->socket,client->world.player.pos[1]-client->world.player.headOffset[1]);
    writeFloat(&client->socket,180-client->world.player.rot[0]*180/M_PI);
    writeFloat(&client->socket,clampf(90-client->world.player.rot[1]*180/M_PI,-90,+90));
    writeBool(&client->socket,client->world.player.flying);

    SDL_UnlockMutex(client->socketLock);

}

public void sendPlayerDigging(Client* client, byte status, Vec4i location_and_face)
{

    SDL_LockMutex(client->socketLock);

    writeByte(&client->socket,0x0E);

    writeByte(&client->socket,status);
    writeInt(&client->socket,location_and_face[0]);
    writeByte(&client->socket,location_and_face[2]);
    writeInt(&client->socket,location_and_face[1]);
    writeByte(&client->socket,location_and_face[3]);

    SDL_UnlockMutex(client->socketLock);

}

typedef void PacketHandler(Client* client);

private void readLoginRequest(Client* client)
{
    PACKET_DEBUG_START(0x00, "Login Request\n");
    readInt(&client->socket);
    readString16(&client->socket);
    readLong(&client->socket);
    readByte(&client->socket);
}

private void readHandshake(Client* client)
{
    PACKET_DEBUG_START(0x00, "Handshake\n");
    readString16(&client->socket);
}

private void readChatMessage(Client* client)
{
    PACKET_DEBUG_START(0x00, "Chat Message\n");
    readString16(&client->socket);
}

private void readTimeUpdate(Client* client)
{
    PACKET_DEBUG_START(0x00, "Time Update\n");
    uint64_t ticks=readLong(&client->socket);

    SDL_LockMutex(client->worldLock);
    client->world.lastSyncTicks=ticks;
    client->world.lastSyncTime=SDL_GetTicks();
    SDL_UnlockMutex(client->worldLock);

}

private void readEntityEquipment(Client* client)
{
    PACKET_DEBUG_START(0x00, "Entity Equipment\n");
    readInt(&client->socket);
    readShort(&client->socket);
    readShort(&client->socket);
    readShort(&client->socket);
}

private void readSpawnPosition(Client* client)
{
    PACKET_DEBUG_START(0x00, "Spawn Position\n");
    readInt(&client->socket);
    readInt(&client->socket);
    readInt(&client->socket);
}

private void readUpdateHealth(Client* client)
{
    PACKET_DEBUG_START(0x00, "Update Health\n");
    readShort(&client->socket);
}

private void readPlayerPositionLook(Client* client)
{

    PACKET_DEBUG_START(0x00, "Player Position & Look\n");

    double x=readDouble(&client->socket);
    double stance=readDouble(&client->socket);
    double y=readDouble(&client->socket);
    double z=readDouble(&client->socket);
    float yaw=readFloat(&client->socket);
    float pitch=readFloat(&client->socket);
    bool on_ground=readBool(&client->socket);

    printf("(%f, %f, %f) %f (%f, %f) %i\n",x,y,z,stance,yaw,pitch,on_ground);


    SDL_LockMutex(client->worldLock);


    client->world.player.pos[0]=x+client->world.player.headOffset[0];
    client->world.player.pos[2]=y+client->world.player.headOffset[2];
    client->world.player.pos[1]=z+client->world.player.headOffset[1];
    client->world.player.stance=stance;
    client->world.player.rot[0]=yaw/180.0*M_PI;
    client->world.player.rot[1]=pitch/180.0*M_PI;
    client->world.player.flying=on_ground;

    //SDL_Delay(100);
    //client->world.player.on_ground=true;
    //sendPlayerPositionAndLook(client);

    //sendPlayerPositionAndLook(client);
    SDL_UnlockMutex(client->worldLock);

    stage =3;

}

private void readAnimation(Client* client)
{
    readInt(&client->socket);
    readByte(&client->socket);
}

private void readNamedEntitySpawn(Client* client)
{
    PACKET_DEBUG_START(0x00, "Named Entity Spawn\n");
    int id=readInt(&client->socket);
    char* name=readString16(&client->socket);
    int x=readInt(&client->socket);
    int z=readInt(&client->socket);
    int y=readInt(&client->socket);
    int rotation=readByte(&client->socket);
    int pitch=readByte(&client->socket);
    int currentItem=readShort(&client->socket);

    SDL_LockMutex(client->worldLock);

    printf("%i %s connected (%i %i %i) (%i %i %i)\n",id, name,x,y,z,rotation,pitch,currentItem);

    SDL_UnlockMutex(client->worldLock);

}

private void readPickupSpawn(Client* client)
{
    PACKET_DEBUG_START(0x00, "Pickup Spawn\n");
    readInt(&client->socket);
    readShort(&client->socket);
    readByte(&client->socket);
    readShort(&client->socket);
    readInt(&client->socket);
    readInt(&client->socket);
    readInt(&client->socket);
    readByte(&client->socket);
    readByte(&client->socket);
    readByte(&client->socket);
}

private void readCollectItem(Client* client)
{
    PACKET_DEBUG_START(0x16,"Collect Item");

    readInt(&client->socket);
    readInt(&client->socket);
}

private void readAddObject(Client* client)
{
    PACKET_DEBUG_START(0x17,"Add Object/Vehicle");
    readInt(&client->socket);
    readByte(&client->socket);
    readInt(&client->socket);
    readInt(&client->socket);
    readInt(&client->socket);
    int flag=readInt(&client->socket);
    if(flag)
    {
        readShort(&client->socket);
        readShort(&client->socket);
        readShort(&client->socket);
    }
}

private void readMobSpawn(Client* client)
{
    PACKET_DEBUG_START(0x00, "Mob Spawn\n");
    readInt(&client->socket);
    readByte(&client->socket);
    readInt(&client->socket);
    readInt(&client->socket);
    readInt(&client->socket);
    readByte(&client->socket);
    readByte(&client->socket);
    readStream(&client->socket);
}

private void readEntityVelocity(Client* client)
{
    PACKET_DEBUG_START(0x00, "Entity Velocity?\n");
    readInt(&client->socket);
    readShort(&client->socket);
    readShort(&client->socket);
    readShort(&client->socket);
}

private void readDestroyEnity(Client* client)
{
    PACKET_DEBUG_START(0x00, "Destroy Entity\n");
    readInt(&client->socket);
}

private void readEntityRelativeMove(Client* client)
{
    PACKET_DEBUG_START(0x00, "Entity Relative Move\n");
    readInt(&client->socket);
    readByte(&client->socket);
    readByte(&client->socket);
    readByte(&client->socket);
}

private void readEntityLookAndRelativeMove(Client* client)
{
    PACKET_DEBUG_START(0x00, "Entity Look and Relative Move\n");
    readInt(&client->socket);
    readByte(&client->socket);
    readByte(&client->socket);
    readByte(&client->socket);
    readByte(&client->socket);
    readByte(&client->socket);
}

private void readEntityTeleport(Client* client)
{
    PACKET_DEBUG_START(0x00, "Entity Teleport\n");
    readInt(&client->socket);
    readInt(&client->socket);
    readInt(&client->socket);
    readInt(&client->socket);
    readByte(&client->socket);
    readByte(&client->socket);
}

private void readEntityStatus(Client* client)
{
    PACKET_DEBUG_START(0x00, "Entity Status?\n");
    readInt(&client->socket);
    readByte(&client->socket);
}

private void readEntityMetadata(Client* client)
{
    PACKET_DEBUG_START(0x00, "Entity Metadata\n");
    readInt(&client->socket);
    readStream(&client->socket);
}

private void readPreChunk(Client* client)
{
    PACKET_DEBUG_START(0x00, "Pre-Chunk");
    int x=readInt(&client->socket);
    int y=readInt(&client->socket);
    readBool(&client->socket);
}

private byte getNibble(byte* array, int pos)
{
    return (pos%2?array[pos/2]>>4:array[pos/2]&0xf);
}

private void readMapChunk(Client* client)
{

    PACKET_DEBUG_START(0x33,"Map Chunk");

    int z0=readInt(&client->socket);
    int y0=readShort(&client->socket);
    int x0=readInt(&client->socket);
    int size_z=readByte(&client->socket)+1;
    int size_y=readByte(&client->socket)+1;
    int size_x=readByte(&client->socket)+1;

    size_t compressed_size=readInt(&client->socket);
    byte compressed[compressed_size];
    socketRead(&client->socket,compressed,compressed_size);

    size_t uncompressed_size=size_x*size_y*size_z*5/2;

    byte uncompressed[uncompressed_size];

    if(uncompress(uncompressed,&uncompressed_size,compressed,sizeof(compressed)) != Z_OK)
    {
        panic("uncompress error");
    }

    assert(sizeof(uncompressed)==uncompressed_size);

    printf("map chunk: (%i %i %i) (%i %i %i)\n",x0,y0,z0,size_x,size_y,size_z);

    SDL_LockMutex(client->worldLock);

    int size=size_x*size_y*size_z;

    byte* ids=uncompressed;
    byte* metadata=uncompressed+size;
    byte* light=metadata+size/2;
    byte* skyLight=light+size/2;



    for(int z=0;z<size_z;z++)
    for(int x=0;x<size_x;x++)
    for(int y=0;y<size_y;y++)
    {

        int pos=y+size_y*(x+size_x*z);

        Block block={
            .id=ids[pos],
            .metadata=getNibble(metadata,pos),
            .light=getNibble(light,pos)/(float)0xf,
            .skyLight=getNibble(skyLight,pos)/(float)0xf,
        };

        worldSet(&client->world, (Vec4i){z0+z,x0+x,y0+y}, block);


    }


    SDL_UnlockMutex(client->worldLock);

}

private void readMultiBlockChange(Client* client)
{

    PACKET_DEBUG_START(0x35,"Multi Block Change");

    int chunk_x=readInt(&client->socket);
    int chunk_y=readInt(&client->socket);
    int count=readShort(&client->socket);

    uint16_t coord[count];
    for(int i=0;i<count;i++)
        coord[i]=readShort(&client->socket);

    byte id[count];
    for(int i=0;i<count;i++)
        id[i]=readByte(&client->socket);

    byte metadata[count];
    for(int i=0;i<count;i++)
        metadata[i]=readByte(&client->socket);

    SDL_LockMutex(client->worldLock);

    for(int i=0;i<count;i++)
    {
        int x=chunk_x+((coord[i]>>12)&0xf);
        int y=chunk_y+((coord[i]>>8)&0xf);
        int z=coord[i]&0xff;

        Block b=worldGet(&client->world,(Vec4i){x,y,z});

        b.id=id[i];
        b.metadata=metadata[i];

        worldSet(&client->world,(Vec4i){x,y,z},b);
    }

    SDL_UnlockMutex(client->worldLock);

}

private void readBlockChange(Client* client)
{
    PACKET_DEBUG_START(0x00, "Block Change\n");
    int x=readInt(&client->socket);
    int z=readByte(&client->socket);
    int y=readInt(&client->socket);
    byte id=readByte(&client->socket);
    byte metadata=readByte(&client->socket);

    Block b=worldGet(&client->world,(Vec4i){x,y,z});
    b.id=id;
    b.metadata=metadata;
    worldSet(&client->world,(Vec4i){x,y,z},b);
}

private void readExplosion(Client* client)
{
    PACKET_DEBUG_START(0x00, "Explosion\n");
    readDouble(&client->socket);
    readDouble(&client->socket);
    readDouble(&client->socket);
    readFloat(&client->socket);
    int count=readInt(&client->socket);
    for(int i=0;i<count;i++)
    {
        readByte(&client->socket);
        readByte(&client->socket);
        readByte(&client->socket);
    }
}

private void readSoundEffect(Client* client)
{
    readInt(&client->socket);
    readInt(&client->socket);
    readByte(&client->socket);
    readInt(&client->socket);
    readInt(&client->socket);
}

private void readInvalidState(Client* client)
{
    PACKET_DEBUG_START(0x00, "New/Invalid State\n");
    readByte(&client->socket);
}

private void readSetSlot(Client* client)
{
    PACKET_DEBUG_START(0x00, "Set slot\n");

    readByte(&client->socket);
    readShort(&client->socket);
    int item_id=readShort(&client->socket);

    if(item_id!=0xffff)
    {
        readByte(&client->socket);
        readShort(&client->socket);
    }

}

private void readWindowItems(Client* client)
{
    PACKET_DEBUG_START(0x00, "Window items\n");

    readByte(&client->socket);
    int count=readShort(&client->socket);

    //printf("\t%i items\n",count);

    for(int i=0;i<count;i++)
    {
        uint16_t item_id=readShort(&client->socket);

        if(item_id!=0xffff)
        {
            int stack=readByte(&client->socket);
            int uses=readShort(&client->socket);
        }
        else
        {
            //printf("\t\tempty\n");
        }

    }

}

private void readIncrementStatistic(Client* client)
{
    PACKET_DEBUG_START(0x00, "Increment Statistic\n");
    readInt(&client->socket);
    readByte(&client->socket);
}

private void readUpdateSign(Client* client)
{
    readInt(&client->socket);
    readShort(&client->socket);
    readInt(&client->socket);
    readString16(&client->socket);
    readString16(&client->socket);
    readString16(&client->socket);
    readString16(&client->socket);
}

private void readKick(Client* client)
{
    PACKET_DEBUG_START(0x00, "Kick\n");
    readString16(&client->socket);
    exit(0);
}

static PacketHandler* packetHandlers[]={
    [0x03] = readChatMessage,
    [0x04] = readTimeUpdate,
    [0x05] = readEntityEquipment,
    [0x06] = readSpawnPosition,

    [0x08] = readUpdateHealth,

    [0x0d] = readPlayerPositionLook,

    [0x12] = readAnimation,

    [0x14] = readNamedEntitySpawn,
    [0x15] = readPickupSpawn,
    [0x16] = readCollectItem,
    [0x17] = readAddObject,
    [0x18] = readMobSpawn,

    [0x1c] = readEntityVelocity,
    [0x1d] = readDestroyEnity,

    [0x1f] = readEntityRelativeMove,
    [0x21] = readEntityLookAndRelativeMove,
    [0x22] = readEntityTeleport,
    [0x26] = readEntityStatus,
    [0x28] = readEntityMetadata,
    [0x32] = readPreChunk,
    [0x33] = readMapChunk,
    [0x34] = readMultiBlockChange,
    [0x35] = readBlockChange,
    [0x3c] = readExplosion,
    [0x3d] = readSoundEffect,
    [0x46] = readInvalidState,
    [0x67] = readSetSlot,
    [0x68] = readWindowItems,
    [0xc8] = readIncrementStatistic,
    [0x82] = readUpdateSign,
    [0xff] = readKick,
};

private int updateThread(void* data)
{

    Client* client=(Client*)data;
    assert(client!=NULL);

    while(true)
    {
        SDL_LockMutex(client->worldLock);
        SDL_LockMutex(client->socketLock);
        sendPlayerPositionAndLook(client);
        SDL_UnlockMutex(client->worldLock);
        socketFlush(&client->socket);
        SDL_UnlockMutex(client->socketLock);
        SDL_Delay(50);
    }

    return 0;

}

public int networkMain(void* data)
{

    Client* client=(Client*)data;
    assert(client!=NULL);

    SDL_LockMutex(client->socketLock);

    socketInit();

    socketOpen(&client->socket,"127.0.0.1",25565) or panic("unable to connect");

    sendHandShake(client);
    socketFlush(&client->socket);

    assert(readByte(&client->socket)==0x02);
    readHandshake(client);
    sendLoginRequest(client);
    socketFlush(&client->socket);

    assert(readByte(&client->socket)==0x01);
    readLoginRequest(client);

    SDL_Thread* thread=NULL;

    SDL_UnlockMutex(client->socketLock);

    while(true)
    {

        //socketFlush(&client->socket);

        //SDL_UnlockMutex(client->socketLock);
        byte packet_id=readByte(&client->socket);



        PacketHandler* handler=packetHandlers[packet_id];

        if(handler!=NULL)
        {
            (*handler)(client);
        }
        else
        {
            panic("unknown packet id: 0x%02x",(unsigned int)packet_id);
        }

        if(stage>=3 && thread==NULL)
        {
            thread=SDL_CreateThread(updateThread, client);
        }

    }

}

