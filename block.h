
#pragma once

#include <stdbool.h>

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

extern BlockDefinition block_definition[];
