
#pragma once

#include "math.h"
#include "noise.h"
#include "actor.h"
#include "socket.h"
#include "block.h"

#include <stdbool.h>
#include <glew.h>
#include <SDL.h>
#include <FTGL/ftgl.h>

#define SEGMENT_BITS 4
#define SEGMENT_MASK ((1<<SEGMENT_BITS)-1)
#define SEGMENT_SIZE (1<<SEGMENT_BITS)
#define SEGMENT_SIZEV (Vec4i){SEGMENT_SIZE,SEGMENT_SIZE,SEGMENT_SIZE,SEGMENT_SIZE}

#define VIEW_RANGE 32
#define TEXTURE_SIZE 16

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
	
	uint64_t ticks;

	uint64_t lastSyncTicks;
	uint32_t lastSyncTime;
	uint32_t drawStart;

	Actor player;
	
	//Actor* actors[4096];

	Vec4i scroll;
	
	GLuint terrain;
	
	Noise noise;

	SDL_mutex *lock;
	SDL_mutex *writeLock;
	Socket* socket;

	FTGLfont *font;
	
};

public void worldInit(World* this);
public void worldDisplayInit(World* this);
public void worldLock(World* this);
public void worldUnlock(World* this);
public void worldDestroy(World* this);
public void worldTick(World* this);
public void worldDraw(World* this);
public bool worldEvent(World* this, const SDL_Event* event);
public void worldSet(World* this, Vec4i pos, Block block);
public Block worldGet(World* this, Vec4i pos);


