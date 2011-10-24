
#pragma once

#include "config.h"

#include "actor.h"

typedef struct Client Client;
typedef struct Player Player;

struct Player
{
    Actor;

    struct
    {
        bool on;
        uint64_t start;
        uint64_t lastAnimation;
        Vec4i location;
    }digging;

    char* name;
    int currentItem;

};

public Player* playerNew(void);
void modelDraw(Client* client);
//void playerTick(Client* client, Player* player);
