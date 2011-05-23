
#pragma once

#include "bool.h"
#include "math.h"

#include <glew.h>
#include <SDL.h>
#include <pthread.h>

#define SEGMENT_SIZE 16
#define VIEW_RANGE 16
#define TEXTURE_SIZE 16

typedef struct
{
	int data[SEGMENT_SIZE][SEGMENT_SIZE][SEGMENT_SIZE];
	GLuint list;
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
	
}World;

void worldInit(World* this);
void worldTick(World* this);
void worldDraw(World* this);
bool worldEvent(World* this, const SDL_Event* event);
