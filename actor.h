
#pragma once

#include "vmath.h"
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
    byte type;
}Vehicle;

typedef struct
{
    Actor;
    uint16_t item;
    byte count;
    uint16_t data;
}Pickup;

struct World;
typedef struct World World;

public void actorTick(World* world, Actor* actor, float timeDelta);
public void actorDrawBBox(Actor* actor);
public Pickup* pickupNew(void);
public Mob* mobNew(void);
public Vehicle* vehicleNew(void);
void humanDraw(Actor* actor);

