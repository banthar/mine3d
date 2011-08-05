
#pragma once

#include "config.h"

#include "SDL.h"

#include "world.h"

typedef struct
{

	SDL_Surface* screen;
	SDL_Rect window_rect;
	SDL_Rect fullscreen_rect;
	bool fullscreen;
	bool grab_mouse;

	bool stop;

	SDL_mutex* worldLock;
	World world;

	SDL_mutex* socketLock;
	Socket socket;

}Client;