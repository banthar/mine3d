
#pragma once

#include "bool.h"
#include "math.h"
#include "snoise3.h"

#include <glew.h>
#include <SDL.h>
#include <pthread.h>

#define SEGMENT_BITS 4
#define SEGMENT_MASK ((1<<SEGMENT_BITS)-1)
#define SEGMENT_SIZE (1<<SEGMENT_BITS)

#define VIEW_RANGE 16
#define TEXTURE_SIZE 16

typedef struct
{
	int data[SEGMENT_SIZE][SEGMENT_SIZE][SEGMENT_SIZE];
	GLuint vbo;
	int n;
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
	
	Noise* noise;

}World;

void worldInit(World* this);
void worldDestroy(World* this);
void worldTick(World* this);
void worldDraw(World* this);
bool worldEvent(World* this, const SDL_Event* event);

