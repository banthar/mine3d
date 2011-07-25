
#pragma once

#include <stdbool.h>
#include <std/types.h>

#include "math.h"

typedef struct
{
	Vec4i pos;
	Vec4f color;
	Vec4f normal;
	Vec2f texCoord;
}Vertex;

typedef struct
{
	byte id;
	byte metadata;
	float light;
	float skyLight;
}Block;

typedef enum
{
	DRAW_NONE,
	DRAW_BLOCK,
	DRAW_CACTI,
	DRAW_TREE,
}DrawMode;

typedef enum
{
	COLOR_NONE,
	COLOR_GRASS,
	COLOR_WATER,
}ColorMode;

typedef struct
{
	const char* name;
	bool solid;
	bool transparent;
	DrawMode draw_mode;
	ColorMode color_mode;
	int textures[6];
}BlockDefinition;

public BlockDefinition block_definition[256];


