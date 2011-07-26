
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

private void sendMapChunk(Socket* socket, Vec4i pos, Vec4i size)
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

	for(int x=0;x<size[0];x++)
	for(int y=0;y<size[1];y++)
	for(int z=0;z<size[2];z++)
	{
		int pos=z+size[2]*(x+size[1]*y);
		ids[pos]=z<63;
	}

	memset(metadata,0x0,area/2);
	memset(light,0x0,area/2);
	memset(skyLight,0xff,area/2);

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

	int size=2;

	for(int x=-size;x<size;x++)
	for(int y=-size;y<size;y++)
	{

		sendPreChunk(socket,x,y,true);
		sendMapChunk(socket, (Vec4i){x*16,y*16,0}, (Vec4i){16,16,128});

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



static int clientThread(void* data)
{
	socketInit();
	Socket socket={.fd=(intptr_t)data};

	assert(readByte(&socket)==0x02);
	readHandshake(&socket);
	sendHandshake(&socket);
	socketFlush(&socket);
	assert(readByte(&socket)==0x01);
	readLoginRequest(&socket);
	sendLoginRequest(&socket);
	sendPlayerPositionAndLook(&socket);
	sendWorld(&socket);
	socketFlush(&socket);


	while(true)
	{

		socketFlush(&socket);
		byte packet_id=readByte(&socket);

		PacketHandler* handler=packetHandler[packet_id];

		if(handler==NULL)
		{
			char buf[1024];
			sprintf(buf,"Unknown packet: 0x%02x",packet_id);
			sendKick(&socket,buf);
			socketFlush(&socket);
			break;
		}
		else
		{
			(*handler)(&socket);
		}
	}

	socketFlush(&socket);
	SDL_Delay(100);
	close(socket.fd);

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

	int socket=openListenSock();

	assert(socket>=0);

	while(true)
	{

		int client=accept(socket,NULL,NULL);

		assert(client>=0);

		SDL_CreateThread(clientThread,(void*)(intptr_t)client);

	}
}
