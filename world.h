
#pragma once

#include "config.h"

#include "math.h"
#include "noise.h"
#include "actor.h"
#include "socket.h"
#include "block.h"
#include "player.h"

#include <stdbool.h>
#include <glew.h>
#include <SDL.h>
#include <FTGL/ftgl.h>

#define SEGMENT_BITS 4
#define SEGMENT_MASK ((1<<SEGMENT_BITS)-1)
#define SEGMENT_SIZE (1<<SEGMENT_BITS)
#define SEGMENT_SIZEV (Vec4i){SEGMENT_SIZE,SEGMENT_SIZE,SEGMENT_SIZE,SEGMENT_SIZE}

#define VIEW_RANGE 32
#define TEXTURE_SIZE 16

typedef struct World World;

typedef struct
{
    Block data[SEGMENT_SIZE][SEGMENT_SIZE][SEGMENT_SIZE];
    GLuint vbo;
    int n;
    bool rendered;
    bool empty;

}Segment;

struct World
{
    Segment* segment[VIEW_RANGE][VIEW_RANGE][VIEW_RANGE];

    uint64_t ticks;

    uint64_t lastSyncTicks;
    uint32_t lastSyncTime;
    uint32_t drawStart;

    int maxActors;
    Actor** actor;

    Vec4i scroll;

    GLuint terrain;

    Noise noise;

    FTGLfont *font;

};

public void worldInit(World* this);
public void worldLock(World* this);
public void worldUnlock(World* this);
public void worldDestroy(World* this);
public void worldTick(World* this);
public Vec4i worldRay(World* world, Vec4f pos0, Vec4f normal, int max_length);
public bool worldEvent(World* this, const SDL_Event* event);
public void worldSet(World* this, Vec4i pos, Block block);
public Block worldGet(World* this, Vec4i pos);
public void worldSpiral(World* this, void (f)(World*,int x,int y,int z));
public Segment* newSegment();

public void worldAddActor(World* world, uint eid, Actor* actor);
public Actor* worldRemoveActor(World* world, uint eid);
public Actor* worldGetActor(World* world, uint eid);
