
#pragma once

#include "math.h"
#include <stdbool.h>

typedef struct
{
	Vec4f pos;
	Vec4f size;
	Vec4f v;
	Vec2f rot;
	bool flying;
	Vec4f headOffset;
	double stance;
}Actor;

struct World;
typedef struct World World;

public void actorTick(World* world, Actor* actor);
public void actorDrawBBox(Actor* actor);
