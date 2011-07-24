
#include "socket.h"


#include "SDL.h"
#include "math.h"
#include "SDL_net.h"
#include <std.h>
#include <iconv.h>
#include <zlib.h>

#include "world.h"

#define PACKET_DEBUG_START(id,name) {}

static char* playerName="Player123";

static int stage=0;

static inline void sendLoginRequest(World* world, Socket* socket)
{
	writeByte(socket,0x01);
	writeInt(socket,14);
	writeString16(socket,playerName);
	writeLong(socket,0llu);
	writeByte(socket,0);
}

static inline void sendHandShake(World* world, Socket* socket)
{
	writeByte(socket,0x02);
	writeString16(socket,playerName);
}

static inline void sendPlayerPositionAndLook(World* world, Socket* socket)
{

	world->player.stance=world->player.pos[2]+1.62;

	writeByte(socket,0x0d);
	writeDouble(socket,world->player.pos[0]);
	writeDouble(socket,world->player.pos[2]);
	writeDouble(socket,world->player.stance);
	writeDouble(socket,world->player.pos[1]);
	writeFloat(socket,world->player.rot[0]);
	writeFloat(socket,world->player.rot[1]);
	writeBool(socket,world->player.on_ground);

	//printf("pos: (%f %f %f)\n",world->player.pos[0],world->player.pos[1],world->player.pos[2]);

}

//const byte position[]="\x0d\x40\x21\x00\x00\x00\x00\x00\x00\x40\x50\x40\x00\x00\x00\x00\x00\x40\x50\xa7\xae\x14\x80\x00\x00\x40\x21\x00\x00\x00\x00\x00\x00\xc3\x34\x00\x00\x00\x00\x00\x00\x00";

static inline void debugPacketType(const char* format,...)
{
	//printf("%s",name);
}

typedef void PacketHandler(World* world, Socket* socket);

static inline void readLoginRequest(World* world, Socket* socket)
{
	debugPacketType("Login Request\n");
	readInt(socket);
	readString16(socket);
	readLong(socket);
	readByte(socket);
}

static inline void readHandshake(World* world, Socket* socket)
{
	debugPacketType("Handshake\n");
	readString16(socket);
	sendLoginRequest(world,socket);
}

static inline void readChatMessage(World* world, Socket* socket)
{
	debugPacketType("Chat Message\n");
	readString16(socket);
}

static inline void readTimeUpdate(World* world, Socket* socket)
{
	debugPacketType("Time Update\n");
	readLong(socket);
}

static inline void readEntityEquipment(World* world, Socket* socket)
{
	debugPacketType("Entity Equipment\n");
	readInt(socket);
	readShort(socket);
	readShort(socket);
	readShort(socket);
}

static inline void readSpawnPosition(World* world, Socket* socket)
{
	debugPacketType("Spawn Position\n");
	readInt(socket);
	readInt(socket);
	readInt(socket);
}

static inline void readUpdateHealth(World* world, Socket* socket)
{
	debugPacketType("Update Health\n");
	readShort(socket);
}

static inline void readPlayerPositionLook(World* world, Socket* socket)
{

	debugPacketType("Player Position & Look\n");

	double x=readDouble(socket);
	double y=readDouble(socket);
	double stance=readDouble(socket);
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
	world->player.rot[0]=yaw;
	world->player.rot[1]=pitch;
	world->player.on_ground=on_ground;
	world->player.on_ground=on_ground;

	//SDL_Delay(100);
	//world->player.on_ground=true;
	//sendPlayerPositionAndLook(world,socket);

	//sendPlayerPositionAndLook(world,socket);
	worldUnlock(world);

	stage =3;

}

static inline void readAnimation(World* world, Socket* socket)
{
	readInt(socket);
	readByte(socket);
}

static inline void readNamedEntitySpawn(World* world, Socket* socket)
{
	debugPacketType("Named Entity Spawn\n");
	readInt(socket);
	readString16(socket);
	readInt(socket);
	readInt(socket);
	readInt(socket);
	readByte(socket);
	readByte(socket);
	readShort(socket);
}

static inline void readPickupSpawn(World* world, Socket* socket)
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

static inline void readCollectItem(World* world, Socket* socket)
{
	PACKET_DEBUG_START(0x16,"Collect Item");

	readInt(socket);
	readInt(socket);
}

static inline void readMobSpawn(World* world, Socket* socket)
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

static inline void readEntityVelocity(World* world, Socket* socket)
{
	debugPacketType("Entity Velocity?\n");
	readInt(socket);
	readShort(socket);
	readShort(socket);
	readShort(socket);
}

static inline void readDestroyEnity(World* world, Socket* socket)
{
	debugPacketType("Destroy Entity\n");
	readInt(socket);
}

static inline void readEntityRelativeMove(World* world, Socket* socket)
{
	debugPacketType("Entity Relative Move\n");
	readInt(socket);
	readByte(socket);
	readByte(socket);
	readByte(socket);
}

static inline void readEntityLookAndRelativeMove(World* world, Socket* socket)
{
	debugPacketType("Entity Look and Relative Move\n");
	readInt(socket);
	readByte(socket);
	readByte(socket);
	readByte(socket);
	readByte(socket);
	readByte(socket);
}

static inline void readEntityTeleport(World* world, Socket* socket)
{
	debugPacketType("Entity Teleport\n");
	readInt(socket);
	readInt(socket);
	readInt(socket);
	readInt(socket);
	readByte(socket);
	readByte(socket);
}

static inline void readEntityStatus(World* world, Socket* socket)
{
	debugPacketType("Entity Status?\n");
	readInt(socket);
	readByte(socket);
}

static inline void readEntityMetadata(World* world, Socket* socket)
{
	debugPacketType("Entity Metadata\n");
	readInt(socket);
	readStream(socket);
}

static inline void readPreChunk(World* world, Socket* socket)
{
	SDL_Delay(1);
	int x=readInt(socket);
	int y=readInt(socket);
	readBool(socket);

	debugPacketType("Pre-Chunk: %i %i\n",x,y);

}

static inline void readMapChunk(World* world, Socket* socket)
{

	PACKET_DEBUG_START(0x33,"Map Chunk");

	int x0=readInt(socket);
	int16_t z0=readShort(socket);
	int y0=readInt(socket);
	int size_x=readByte(socket)+1;
	int size_y=readByte(socket)+1;
	int size_z=readByte(socket)+1;

	byte compressed[readInt(socket)];
	socketRead(socket,compressed,sizeof(compressed));

	size_t uncompressed_size=size_x*size_y*size_z*5/2;

	byte uncompressed[uncompressed_size];

	if(uncompress(uncompressed,&uncompressed_size,compressed,sizeof(compressed)) != Z_OK)
	{
		panic("uncompress error");
	}

	assert(sizeof(uncompressed)==uncompressed_size);

	printf("map chunk: %i %i %i\n",x0,y0,z0);

	worldLock(world);

	int pos=0;

	for(int z=0;z<size_z;z++)
	for(int x=0;x<size_x;x++)
	for(int y=0;y<size_y;y++)
	{
		byte block_id=uncompressed[pos];
		pos++;
		worldSet(world, (Vec4i){x0+x,z0+z,y0+y}, (Block){.id=block_id!=0});


	}


	worldUnlock(world);

}

static inline void readMultiBlockChange(World* world, Socket* socket)
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

static inline void readBlockChange(World* world, Socket* socket)
{
	debugPacketType("Block Change\n");
	readInt(socket);
	readByte(socket);
	readInt(socket);
	readByte(socket);
	readByte(socket);
}

static inline void readExplosion(World* world, Socket* socket)
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

static inline void readInvalidState(World* world, Socket* socket)
{
	debugPacketType("New/Invalid State\n");
	readByte(socket);
}

static inline void readSetSlot(World* world, Socket* socket)
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

static inline void readWindowItems(World* world, Socket* socket)
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

static inline void readIncrementStatistic(World* world, Socket* socket)
{
	debugPacketType("Increment Statistic\n");
	readInt(socket);
	readByte(socket);
}

static inline void readKick(World* world, Socket* socket)
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
	[0x46] = readInvalidState,
	[0x67] = readSetSlot,
	[0x68] = readWindowItems,
	[0xc8] = readIncrementStatistic,
	[0xff] = readKick,
};

int networkMain(void* data)
{

	World* world=(World*)data;
	assert(world!=NULL);

	socketInit();

	IPaddress address;

	if(SDLNet_ResolveHost(&address, "localhost",25565) != 0 )
	{
		panic("unable to resolve host");
	}

	Socket socket={0};
	socket.socket = SDLNet_TCP_Open(&address);

	if(socket.socket==NULL)
		panic("unable to connect");

//	SDLNet_TCP_Send(socket,hello,sizeof(hello)-1);

	sendHandShake(world,&socket);

	while(true)
	{

		socketFlush(&socket);
		byte packet_id=readByte(&socket);

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
		worldLock(world);

		world->player.pos[2]-=0.1;

		if(stage>=3)
			sendPlayerPositionAndLook(world,&socket);

		worldUnlock(world);

	}

}

