
#pragma once

#include "socket.h"
#include "world.h"

typedef struct Client Client;

public int networkMain(void* data);
public void sendPlayerDigging(Client* client, byte status, Vec4i location_and_face);
public void sendAnimation(Client* client, int eid, byte animationId);
