
#include "config.h"

#include "socket.h"

typedef void PacketHandler(Socket* socket);

static void readLoginRequest(Socket* socket)
{
	readInt(socket);
	readString16(socket);
	readLong(socket);
	readByte(socket);
}

static void sendLoginRequest(Socket* socket)
{
	writeByte(socket,0x01);
	writeInt(socket,0);
	writeString16(socket,"");
	writeLong(socket,0);
	writeByte(socket,0);
}

static void readHandshake(Socket* socket)
{
	char* name=readString16(socket);
	printf("%s connected\n",name);
	sendLoginRequest(socket);
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

PacketHandler* packetHandler[256]={
	[0x01] = readLoginRequest,
	[0x02] = readHandshake,
	[0x02] = readPlayerPositionAndLook,
};

static int clientThread(void* data)
{
	socketInit();
	Socket socket={.socket=data};

	while(true)
	{

		byte packet_id=readByte(&socket);

		printf("%02x: \n",packet_id);

		PacketHandler* handler=packetHandler[packet_id];

		if(handler==NULL)
			panic("unknown packet %02x\n",packet_id);

		(*handler)(&socket);

		socketFlush(&socket);

	}

	SDLNet_TCP_Close(socket.socket);

	return 0;

}

int main(int argc, char* argv[])
{
	assert(SDLNet_Init()==0);
	IPaddress ip;
	assert(SDLNet_ResolveHost(&ip, NULL, 25565)==0);

	TCPsocket socket=SDLNet_TCP_Open(&ip);

	assert(socket!=NULL);

	SDLNet_SocketSet set= SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(set,socket);

	while(true)
	{

		SDLNet_CheckSockets(set,-1);

		TCPsocket client=SDLNet_TCP_Accept(socket);

		if(client==NULL)
			continue;

		SDL_CreateThread(clientThread,client);

	}
}
