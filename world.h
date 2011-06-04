
#pragma once

#include "bool.h"
#include "math.h"
#include "noise.h"

#include <glew.h>
#include <SDL.h>
#include <pthread.h>

#define SEGMENT_BITS 5
#define SEGMENT_MASK ((1<<SEGMENT_BITS)-1)
#define SEGMENT_SIZE (1<<SEGMENT_BITS)
#define SEGMENT_SIZEV (Vec4i){SEGMENT_SIZE,SEGMENT_SIZE,SEGMENT_SIZE,SEGMENT_SIZE}

#define VIEW_RANGE 32
#define TEXTURE_SIZE 16

typedef struct
{
	int data[SEGMENT_SIZE][SEGMENT_SIZE][SEGMENT_SIZE];
	GLuint vbo;
	int n;
	bool rendered;
	bool solid;
}Segment;

typedef struct
{
	Segment* segment[VIEW_RANGE][VIEW_RANGE][VIEW_RANGE];
	
	struct
	{
		Vec4f pos;
		Vec2f rot;
	}player;
	
	Vec4i scroll;
	
	GLuint terrain;
	pthread_t thread;
	
	Noise noise;

}World;

void worldInit(World* this);
void worldDestroy(World* this);
void worldTick(World* this);
void worldDraw(World* this);
bool worldEvent(World* this, const SDL_Event* event);

