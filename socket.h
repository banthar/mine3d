
#pragma once

#include "SDL_net.h"
#include <stdbool.h>
#include <std/types.h>

typedef struct
{
	TCPsocket socket;
	int buffer_length;
	byte buffer[1024*4];
}Socket;

public uint8_t readByte(Socket* socket);
public uint16_t readShort(Socket* socket);
public uint32_t readInt(Socket* socket);
public uint64_t readLong(Socket* socket);
public bool readBool(Socket* socket);
public float readFloat(Socket* socket);
public double readDouble(Socket* socket);
public void writeByte(Socket* socket, uint8_t n);
public void writeShort(Socket* socket, uint16_t h);
public void writeInt(Socket* socket, uint32_t h);
public void writeLong(Socket* socket, uint64_t h);
public void writeFloat(Socket* socket, float h);
public void writeDouble(Socket* socket, double h);
public void writeBool(Socket* socket, bool n);
public char* readString16(Socket* socket);
public void readStream(Socket* socket);
public void writeString16(Socket* socket, char* utf8_string);
public void socketFlush(Socket* socket);
public void socketWrite(Socket* socket, void* data, int length);
public void socketRead(Socket* socket, void* data, int length);
public void socketInit();





