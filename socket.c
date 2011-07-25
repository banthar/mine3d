
#include "config.h"

#include "socket.h"

#include "SDL_net.h"
#include <iconv.h>
#include <endian.h>

#if __BYTE_ORDER == __LITTLE_ENDIAN 
    #define hton16(x)       __bswap_16(x)
    #define hton32(x)       __bswap_32(x)
    #define hton64(x)       __bswap_64(x)
    #define ntoh16(x)       __bswap_16(x)
    #define ntoh32(x)       __bswap_32(x)
    #define ntoh64(x)       be64toh(x)
#else
    #define hton16(x)       (x)
    #define hton32(x)       (x)
    #define hton64(x)       (x)
    #define ntoh16(x)       (x)
    #define ntoh32(x)       (x)
    #define ntoh64(x)       (x)
#endif


static iconv_t conv_ucs2_to_utf8;
static iconv_t conv_utf8_to_ucs2;

public uint8_t readByte(Socket* socket)
{
	uint8_t n;
	socketRead(socket,&n,sizeof(n));
	return n;
}

public uint16_t readShort(Socket* socket)
{
	uint16_t n;
	socketRead(socket,&n,sizeof(n));
	return ntoh16(n);
}

public uint32_t readInt(Socket* socket)
{
	uint32_t n;
	socketRead(socket,&n,sizeof(n));
	return ntoh32(n);
}

public uint64_t readLong(Socket* socket)
{
	uint64_t n;
	socketRead(socket,&n,sizeof(n));
	return ntoh64(n);
}

public bool readBool(Socket* socket)
{
	switch(readByte(socket))
	{
		case 0:
			return false;
		case 1:
			return true;
		default:
			panic("Invalid bool value");
	}
}

public float readFloat(Socket* socket)
{
	union{float f;uint32_t i;}u;
	u.i=readInt(socket);
	return u.f;
}

public double readDouble(Socket* socket)
{
	union{double f;uint64_t i;}u;
	u.i=readLong(socket);
	return u.f;
}

// Write

public void writeByte(Socket* socket, uint8_t n)
{
	socketWrite(socket, &n, sizeof(n));
}

public void writeShort(Socket* socket, uint16_t h)
{
	uint16_t n=hton16(h);
	socketWrite(socket, &n, sizeof(n));
}

public void writeInt(Socket* socket, uint32_t h)
{
	uint32_t n=hton32(h);
	socketWrite(socket, &n, sizeof(n));
}

public void writeLong(Socket* socket, uint64_t h)
{
	uint64_t n=hton64(h);
	socketWrite(socket, &n, sizeof(n));
}

public void writeFloat(Socket* socket, float h)
{
	uint32_t n;
	float* bytes=(float*)&n;
	*bytes=h;
	writeInt(socket,n);
}

public void writeDouble(Socket* socket, double h)
{
	uint64_t n;
	double* bytes=(double*)&n;
	*bytes=h;
	writeLong(socket,n);
}

public void writeBool(Socket* socket, bool n)
{
	writeByte(socket,n!=false);
}


public char* readString16(Socket* socket)
{

	int length=readShort(socket);

	uint16_t ucs_string[length];

	socketRead(socket,ucs_string,sizeof(ucs_string));

	char utf8_string[length*2+1];
	memset(utf8_string,0,sizeof(utf8_string));

	size_t in_left=length*2;
	size_t out_left=length*2;

	char *in=(char*)ucs_string;
	char *out=utf8_string;

	while(in_left>0)
	{
		iconv(conv_ucs2_to_utf8,&in,&in_left,&out,&out_left);
	}

	//printf("string: '%s'\n",utf8_string);

	return strdup(utf8_string);

}

public void readStream(Socket* socket)
{

	while(true)
	{

		byte type=readByte(socket);

		if(type==0x7f)
			return;

		//printf("\tmeta: %02x %i %i\n",type, type&0x1f, type>>5);

		switch(type>>5)
		{
			case 0:
				readByte(socket);
				break;
			case 1:
				readShort(socket);
				break;
			case 2:
				readInt(socket);
				break;
			case 3:
				readFloat(socket);
				break;
			case 4:
				readString16(socket);
				break;
			case 5:
				readShort(socket);
				readByte(socket);
				readShort(socket);
				break;
			case 6:
				readInt(socket);
				readInt(socket);
				readInt(socket);
				break;
			default:
				panic("unknown metadata 0x%02x (0x%02x)",type,type>>5);
		}

	}

}

public void writeString16(Socket* socket, char* utf8_string)
{
	int length=strlen(utf8_string);

	uint16_t ucs_string[length];

	size_t in_left=length;
	size_t out_left=sizeof(ucs_string);

	char *in=utf8_string;
	char *out=(char*)ucs_string;

	while(in_left>0)
	{
		iconv(conv_utf8_to_ucs2,&in,&in_left,&out,&out_left);
	}

	int ucs_length=length-out_left/2;

	writeShort(socket,ucs_length);

	socketWrite(socket, ucs_string, ucs_length*2);

}

public void socketFlush(Socket* socket)
{

	if(socket->buffer_length==0)
		return;

	int ret=SDLNet_TCP_Send(socket->socket, socket->buffer, socket->buffer_length);

	if(ret!=socket->buffer_length)
	{
		panic("write error");
	}

	socket->buffer_length=0;

}

public void socketWrite(Socket* socket, void* data, int length)
{

	if(socket->buffer_length+length>=sizeof(socket->buffer))
		socketFlush(socket);

	assert(socket->buffer_length+length<sizeof(socket->buffer));

	memcpy(socket->buffer+socket->buffer_length,data,length);
	socket->buffer_length+=length;
	
}

public void socketRead(Socket* socket, void* data, int length)
{
	int ret=SDLNet_TCP_Recv(socket->socket, data, length);

	if(ret!=length)
		panic("read error");
}

public void socketInit()
{
	conv_ucs2_to_utf8=iconv_open("UTF-8","UCS-2BE");
	conv_utf8_to_ucs2=iconv_open("UCS-2BE","UTF-8");
}

