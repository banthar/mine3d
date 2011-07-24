
#pragma once

#include "SDL_net.h"
#include <std.h>

typedef struct
{
	TCPsocket socket;
	int buffer_length;
	byte buffer[1024*4];
}Socket;

uint8_t readByte(Socket* socket);
uint16_t readShort(Socket* socket);
uint32_t readInt(Socket* socket);
uint64_t readLong(Socket* socket);
bool readBool(Socket* socket);
float readFloat(Socket* socket);
double readDouble(Socket* socket);
void writeByte(Socket* socket, uint8_t n);
void writeShort(Socket* socket, uint16_t h);
void writeInt(Socket* socket, uint32_t h);
void writeLong(Socket* socket, uint64_t h);
void writeFloat(Socket* socket, float h);
void writeDouble(Socket* socket, double h);
void writeBool(Socket* socket, bool n);
void readString16(Socket* socket);
void readStream(Socket* socket);
void writeString16(Socket* socket, char* utf8_string);
void socketFlush(Socket* socket);
void socketWrite(Socket* socket, void* data, int length);
void socketRead(Socket* socket, void* data, int length);
void socketInit();





