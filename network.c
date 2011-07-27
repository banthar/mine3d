
#include "config.h"

#include "network.h"

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

private void sendLoginRequest(World* world, Socket* socket)
{
	writeByte(socket,0x01);
	writeInt(socket,14);
	writeString16(socket,playerName);
	writeLong(socket,0llu);
	writeByte(socket,0);
}

private void sendHandShake(World* world, Socket* socket)
{
	writeByte(socket,0x02);
	writeString16(socket,playerName);
}

private void sendPlayerPositionAndLook(World* world, Socket* socket)
{

	world->player.stance=world->player.pos[2]+1.62;

	writeByte(socket,0x0d);
	writeDouble(socket,world->player.pos[0]);
	writeDouble(socket,world->player.pos[2]);
	writeDouble(socket,world->player.stance);
	writeDouble(socket,world->player.pos[1]);
	writeFloat(socket,180-world->player.rot[0]*180/M_PI);
	writeFloat(socket,clampf(90-world->player.rot[1]*180/M_PI,-90,+90));
	writeBool(socket,world->player.on_ground);

}

//const byte position[]="\x0d\x40\x21\x00\x00\x00\x00\x00\x00\x40\x50\x40\x00\x00\x00\x00\x00\x40\x50\xa7\xae\x14\x80\x00\x00\x40\x21\x00\x00\x00\x00\x00\x00\xc3\x34\x00\x00\x00\x00\x00\x00\x00";

private void debugPacketType(const char* format,...)
{
	//printf("%s",name);
}

typedef void PacketHandler(World* world, Socket* socket);

private void readLoginRequest(World* world, Socket* socket)
{
	debugPacketType("Login Request\n");
	readInt(socket);
	readString16(socket);
	readLong(socket);
	readByte(socket);
}

private void readHandshake(World* world, Socket* socket)
{
	debugPacketType("Handshake\n");
	readString16(socket);
	sendLoginRequest(world,socket);
}

private void readChatMessage(World* world, Socket* socket)
{
	debugPacketType("Chat Message\n");
	readString16(socket);
}

private void readTimeUpdate(World* world, Socket* socket)
{
	debugPacketType("Time Update\n");
	uint64_t ticks=readLong(socket);

	worldLock(world);
	world->lastSyncTicks=ticks;
	world->lastSyncTime=SDL_GetTicks();
	worldUnlock(world);

}

private void readEntityEquipment(World* world, Socket* socket)
{
	debugPacketType("Entity Equipment\n");
	readInt(socket);
	readShort(socket);
	readShort(socket);
	readShort(socket);
}

private void readSpawnPosition(World* world, Socket* socket)
{
	debugPacketType("Spawn Position\n");
	readInt(socket);
	readInt(socket);
	readInt(socket);
}

private void readUpdateHealth(World* world, Socket* socket)
{
	debugPacketType("Update Health\n");
	readShort(socket);
}

private void readPlayerPositionLook(World* world, Socket* socket)
{

	debugPacketType("Player Position & Look\n");

	double x=readDouble(socket);
	double stance=readDouble(socket);
	double y=readDouble(socket);
	double z=readDouble(socket);
	float yaw=readFloat(socket);
	float pitch=readFloat(socket);
	bool on_ground=readBool(socket);

	printf("(%f, %f, %f) %f (%f, %f) %i\n",x,y,z,stance,yaw,pitch,on_ground);


	worldLock(world);


	world->player.pos[0]=x;
	world->player.pos[2]=y;
	world->player.pos[1]=z;
	world->player.stance=stance;
	world->player.rot[0]=yaw/180.0*M_PI;
	world->player.rot[1]=pitch/180.0*M_PI;
	world->player.on_ground=on_ground;
	world->player.on_ground=on_ground;

	//SDL_Delay(100);
	//world->player.on_ground=true;
	//sendPlayerPositionAndLook(world,socket);

	//sendPlayerPositionAndLook(world,socket);
	worldUnlock(world);

	stage =3;

}

private void readAnimation(World* world, Socket* socket)
{
	readInt(socket);
	readByte(socket);
}

private void readNamedEntitySpawn(World* world, Socket* socket)
{
	debugPacketType("Named Entity Spawn\n");
	int id=readInt(socket);
	char* name=readString16(socket);
	int x=readInt(socket);
	int z=readInt(socket);
	int y=readInt(socket);
	int rotation=readByte(socket);
	int pitch=readByte(socket);
	int currentItem=readShort(socket);

	worldLock(world);

	printf("%i %s connected (%i %i %i) (%i %i %i)\n",id, name,x,y,z,rotation,pitch,currentItem);

	worldUnlock(world);

}

private void readPickupSpawn(World* world, Socket* socket)
{
	debugPacketType("Pickup Spawn\n");
	readInt(socket);
	readShort(socket);
	readByte(socket);
	readShort(socket);
	readInt(socket);
	readInt(socket);
	readInt(socket);
	readByte(socket);
	readByte(socket);
	readByte(socket);
}

private void readCollectItem(World* world, Socket* socket)
{
	PACKET_DEBUG_START(0x16,"Collect Item");

	readInt(socket);
	readInt(socket);
}

private void readAddObject(World* world, Socket* socket)
{
	PACKET_DEBUG_START(0x17,"Add Object/Vehicle");
	readInt(socket);
	readByte(socket);
	readInt(socket);
	readInt(socket);
	readInt(socket);
	int flag=readInt(socket);
	if(flag)
	{
		readShort(socket);
		readShort(socket);
		readShort(socket);
	}
}

private void readMobSpawn(World* world, Socket* socket)
{
	debugPacketType("Mob Spawn\n");
	readInt(socket);
	readByte(socket);
	readInt(socket);
	readInt(socket);
	readInt(socket);
	readByte(socket);
	readByte(socket);
	readStream(socket);
}

private void readEntityVelocity(World* world, Socket* socket)
{
	debugPacketType("Entity Velocity?\n");
	readInt(socket);
	readShort(socket);
	readShort(socket);
	readShort(socket);
}

private void readDestroyEnity(World* world, Socket* socket)
{
	debugPacketType("Destroy Entity\n");
	readInt(socket);
}

private void readEntityRelativeMove(World* world, Socket* socket)
{
	debugPacketType("Entity Relative Move\n");
	readInt(socket);
	readByte(socket);
	readByte(socket);
	readByte(socket);
}

private void readEntityLookAndRelativeMove(World* world, Socket* socket)
{
	debugPacketType("Entity Look and Relative Move\n");
	readInt(socket);
	readByte(socket);
	readByte(socket);
	readByte(socket);
	readByte(socket);
	readByte(socket);
}

private void readEntityTeleport(World* world, Socket* socket)
{
	debugPacketType("Entity Teleport\n");
	readInt(socket);
	readInt(socket);
	readInt(socket);
	readInt(socket);
	readByte(socket);
	readByte(socket);
}

private void readEntityStatus(World* world, Socket* socket)
{
	debugPacketType("Entity Status?\n");
	readInt(socket);
	readByte(socket);
}

private void readEntityMetadata(World* world, Socket* socket)
{
	debugPacketType("Entity Metadata\n");
	readInt(socket);
	readStream(socket);
}

private void readPreChunk(World* world, Socket* socket)
{
	SDL_Delay(1);
	int x=readInt(socket);
	int y=readInt(socket);
	readBool(socket);

	debugPacketType("Pre-Chunk: %i %i\n",x,y);

}

private byte getNibble(byte* array, int pos)
{
	return (pos%2?array[pos/2]>>4:array[pos/2]&0xf);
}

private void readMapChunk(World* world, Socket* socket)
{

	PACKET_DEBUG_START(0x33,"Map Chunk");

	int z0=readInt(socket);
	int y0=readShort(socket);
	int x0=readInt(socket);
	int size_z=readByte(socket)+1;
	int size_y=readByte(socket)+1;
	int size_x=readByte(socket)+1;

	size_t compressed_size=readInt(socket);
	byte compressed[compressed_size];
	socketRead(socket,compressed,compressed_size);

	size_t uncompressed_size=size_x*size_y*size_z*5/2;

	byte uncompressed[uncompressed_size];

	if(uncompress(uncompressed,&uncompressed_size,compressed,sizeof(compressed)) != Z_OK)
	{
		panic("uncompress error");
	}

	assert(sizeof(uncompressed)==uncompressed_size);

	printf("map chunk: (%i %i %i) (%i %i %i)\n",x0,y0,z0,size_x,size_y,size_z);

	worldLock(world);

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

		worldSet(world, (Vec4i){z0+z,x0+x,y0+y}, block);


	}


	worldUnlock(world);

}

private void readMultiBlockChange(World* world, Socket* socket)
{
	debugPacketType("Multi Block Change\n");

	readInt(socket);
	readInt(socket);
	int count=readShort(socket);

	for(int i=0;i<count;i++)
		readShort(socket);
	for(int i=0;i<count;i++)
		readByte(socket);
	for(int i=0;i<count;i++)
		readByte(socket);

}

private void readBlockChange(World* world, Socket* socket)
{
	debugPacketType("Block Change\n");
	readInt(socket);
	readByte(socket);
	readInt(socket);
	readByte(socket);
	readByte(socket);
}

private void readExplosion(World* world, Socket* socket)
{
	debugPacketType("Explosion\n");
	readDouble(socket);
	readDouble(socket);
	readDouble(socket);
	readFloat(socket);
	int count=readInt(socket);
	for(int i=0;i<count;i++)
	{
		readByte(socket);
		readByte(socket);
		readByte(socket);
	}
}

private void readSoundEffect(World* world, Socket* socket)
{
	readInt(socket);
	readInt(socket);
	readByte(socket);
	readInt(socket);
	readInt(socket);
}

private void readInvalidState(World* world, Socket* socket)
{
	debugPacketType("New/Invalid State\n");
	readByte(socket);
}

private void readSetSlot(World* world, Socket* socket)
{
	debugPacketType("Set slot\n");

	readByte(socket);
	readShort(socket);
	int item_id=readShort(socket);

	if(item_id!=0xffff)
	{
		readByte(socket);
		readShort(socket);
	}

}

private void readWindowItems(World* world, Socket* socket)
{
	debugPacketType("Window items\n");
	
	readByte(socket);
	int count=readShort(socket);

	//printf("\t%i items\n",count);

	for(int i=0;i<count;i++)
	{
		uint16_t item_id=readShort(socket);

		if(item_id!=0xffff)
		{
			int stack=readByte(socket);
			int uses=readShort(socket);
			debugPacketType("\t\t%i %i %i\n",item_id,stack,uses);
		}
		else
		{
			//printf("\t\tempty\n");
		}

	}

}

private void readIncrementStatistic(World* world, Socket* socket)
{
	debugPacketType("Increment Statistic\n");
	readInt(socket);
	readByte(socket);
}

private void readKick(World* world, Socket* socket)
{
	debugPacketType("Kick\n");
	readString16(socket);
	exit(0);
}

static PacketHandler* packetHandlers[]={
	[0x01] = readLoginRequest,
	[0x02] = readHandshake,
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
	[0xff] = readKick,
};

private int updateThread(void* data)
{

	World* world=(World*)data;
	assert(world!=NULL);

	while(true)
	{
		SDL_LockMutex(world->writeLock);
		SDL_LockMutex(world->lock);
		sendPlayerPositionAndLook(world, world->socket);
		SDL_UnlockMutex(world->lock);
		socketFlush(world->socket);
		SDL_UnlockMutex(world->writeLock);
		SDL_Delay(50);
	}

	return 0;

}

public int networkMain(void* data)
{

	World* world=(World*)data;
	assert(world!=NULL);

	socketInit();

	Socket socket={0};
	socketOpen(&socket,"127.0.0.1",25565) or panic("unable to connect");


//	SDLNet_TCP_Send(socket,hello,sizeof(hello)-1);

	sendHandShake(world,&socket);

	SDL_Thread* thread=NULL;

	printf("world: %p\n",world);
	printf("writeLock: %p\n",world->writeLock);
	SDL_LockMutex(world->writeLock);

	while(true)
	{

		socketFlush(&socket);
		SDL_UnlockMutex(world->writeLock);
		byte packet_id=readByte(&socket);

		SDL_LockMutex(world->writeLock);

		//printf("%02x: \n",packet_id);

		PacketHandler* handler=packetHandlers[packet_id];

		if(handler!=NULL)
		{
			(*handler)(world,&socket);
		}
		else
		{
			panic("unknown packet id: 0x%02x",(unsigned int)packet_id);
		}

		//SDL_Delay(1);
		//SDL_LockMutex(world->lock);


		if(stage>=3 && thread==NULL)
		{
			world->socket=&socket;
			thread=SDL_CreateThread(updateThread, world);
		}

		//SDL_UnlockMutex(world->lock);

		

	}

}

