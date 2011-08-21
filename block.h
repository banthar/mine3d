
#pragma once

#include <stdbool.h>

#include "vmath.h"

typedef struct
{
    Vec4i pos;
    Vec4f color;
    Vec4f normal;
    Vec2f texCoord;
}Vertex;

typedef struct
{
    int size;
    int maxSize;
    Vertex* data;
}VertexBuffer;

typedef struct
{
    byte id;
    byte metadata;
    byte light;
    byte skyLight;
}Block;

typedef enum
{
    DRAW_NONE,
    DRAW_BLOCK,
    DRAW_CACTI,
    DRAW_TREE,
    DRAW_GRASS,
    DRAW_RAIL,
    DRAW_STAIRS,
}DrawMode;

typedef enum
{
    COLOR_NONE,
    COLOR_GRASS,
    COLOR_WOOL,
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

typedef struct World World;

void blockDraw(World* world, Vec4i loc, VertexBuffer* buffer);

