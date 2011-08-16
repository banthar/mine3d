
#pragma once

#include "config.h"

#include "math.h"

typedef struct Box Box;

struct Box
{
    int bone;
    Vec4f offset;
    Vec4f size;
    Vec2f textureOffset;
    int childs;
    struct{
        Vec4f offset;
        Box* box;
    }child[];
};

typedef struct Model Model;

struct Model
{
};

public void drawBox(Box* boxes, Vec4f* rotations);

