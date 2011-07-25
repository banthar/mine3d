
#pragma once

#include "math.h"
#include "noise.h"
#include "actor.h"
#include "socket.h"

#include <stdbool.h>
#include <glew.h>
#include <SDL.h>

#define SEGMENT_BITS 4
#define SEGMENT_MASK ((1<<SEGMENT_BITS)-1)
#define SEGMENT_SIZE (1<<SEGMENT_BITS)
#define SEGMENT_SIZEV (Vec4i){SEGMENT_SIZE,SEGMENT_SIZE,SEGMENT_SIZE,SEGMENT_SIZE}

#define VIEW_RANGE 32
#define TEXTURE_SIZE 16

typedef struct
{
	byte id;
	byte metadata;
	float light;
	float skyLight;
}Block;

typedef struct
{
	Block data[SEGMENT_SIZE][SEGMENT_SIZE][SEGMENT_SIZE];
	GLuint vbo;
	int n;
	bool rendered;
	bool empty;

}Segment;

struct World
{
	Segment* segment[VIEW_RANGE][VIEW_RANGE][VIEW_RANGE];
	
	int time;
	
	Actor player;
	
	//Actor* actors[4096];

	Vec4i scroll;
	
	GLuint terrain;
	
	Noise noise;

	SDL_mutex *lock;
	SDL_mutex *writeLock;
	Socket* socket;
	
};

void worldInit(World* this);
void worldLock(World* this);
void worldUnlock(World* this);
void worldDestroy(World* this);
void worldTick(World* this);
void worldDraw(World* this);
bool worldEvent(World* this, const SDL_Event* event);
void worldSet(World* this, Vec4i pos, Block block);
Block worldGet(World* this, Vec4i pos);


