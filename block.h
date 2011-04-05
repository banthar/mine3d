
#pragma once

#include <glew.h>
#include "math.h"

typedef struct
{
	int data[16][16][16];
	GLuint list;
}Segment;

typedef struct
{
	Segment segments[16][16][16];
	
	struct
	{
		vec4f pos;
		vec2f rot;
	}player;
	
}Map;

void randomize();
void draw();
void init();
void drawSegment(const Segment* segment);
void randomizeSegment(Segment* segment);
Segment* loadSegment(int x, int y, int z);
