
#pragma once

#include "math.h"
#include <stdbool.h>

typedef struct
{
    int actorType;
    bool draw;
    Vec4f pos;
    Vec4f size;
    Vec4f v;
    Vec2f rot;
    bool flying;
    Vec4f headOffset;
    double stance;
}Actor;

typedef struct
{
    Actor;
    byte type;
}Mob;

typedef struct
{
    Actor;
    uint16_t item;
    byte count;
    uint16_t data;
}Pickup;

struct World;
typedef struct World World;

public void actorTick(World* world, Actor* actor);
public void actorDrawBBox(Actor* actor);
public Pickup* pickupNew();
public Mob* mobNew();
void humanDraw(Actor* actor);

