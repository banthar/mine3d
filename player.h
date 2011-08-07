
#pragma once

#include "config.h"

#include "actor.h"

typedef struct Player Player;

struct Player
{
    Actor;

    struct
    {
        bool on;
        uint64_t start;
        Vec4i location;
    }digging;

};

