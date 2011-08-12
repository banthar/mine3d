
#pragma once

#include "config.h"

#include "math.h"

typedef struct Box Box;

struct Box
{
    Vec4f offset;
    Vec4f size;
    Vec2f textureOffset;
    int childs;
    struct{
        Vec4f offset;
        Box* box;
    }child[];
};

public void drawBox(Box* box);

