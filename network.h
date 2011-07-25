
#pragma once

#include "socket.h"
#include "world.h"

int networkMain(void* data);
void sendPlayerPositionAndLook(World* world, Socket* socket);
