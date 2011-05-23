
#include "bool.h"
#include "error.h"
#include "world.h"

#include <SDL.h>
#include <glew.h>

SDL_Surface* screen;
bool fullscreen=false;
SDL_Rect window_rect={0,0,640,480};
SDL_Rect fullscreen_rect;
bool grab_mouse=false;

void grabMouse()
{
	SDL_ShowCursor(!grab_mouse);
	SDL_WM_GrabInput(grab_mouse);	
}


void initVideo()
{
	if(fullscreen)
		screen=SDL_SetVideoMode(fullscreen_rect.w,fullscreen_rect.h,0,SDL_OPENGL|SDL_FULLSCREEN);
	else
		screen=SDL_SetVideoMode(window_rect.w,window_rect.h,0,SDL_OPENGL|SDL_RESIZABLE);

	if(screen==NULL)
		panic("unable to set video mode");

	grabMouse();

	glViewport(0,0,screen->w,screen->h);

	float aspect=(float)screen->w/(float)screen->h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float d=0.1;

	glFrustum(-aspect*d,aspect*d,1*d,-1*d,0.1,300);

}

__attribute__((noreturn)) void quit()
{
	SDL_Quit();
	exit(0);
}

bool handleEvent(const SDL_Event* event)
{
	switch(event->type)
	{
		case SDL_QUIT:
			quit();
		case SDL_KEYDOWN:
			switch(event->key.keysym.sym)
			{
				case SDLK_ESCAPE:
					grab_mouse=!grab_mouse;
					fullscreen&=grab_mouse;
					initVideo();
					return false;
				case SDLK_F10:
					quit();
				case SDLK_F11:
					fullscreen=!fullscreen;
					grab_mouse|=fullscreen;
					initVideo();
					return false;
				default:
					return true;
			}
			break;
		case SDL_VIDEORESIZE:
			window_rect=(SDL_Rect){0,0,event->resize.w,event->resize.h};
			initVideo();
			return false;
		default:
			return true;
	}
}

int main(int argc, char* argv[])
{

	SDL_Init(SDL_INIT_VIDEO);

	const SDL_VideoInfo* video_info=SDL_GetVideoInfo();
	fullscreen_rect=(SDL_Rect){0,0,video_info->current_w,video_info->current_h};

	initVideo();

	if(glewInit()!=GLEW_OK)
		panic("glew error");

	World world;
	worldInit(&world);
		
	while(true)
	{
		
		SDL_Event event;

		while(SDL_PollEvent(&event))
		{
			if(handleEvent(&event))
				worldEvent(&world,&event);
		}

		int t=SDL_GetTicks();

		worldTick(&world);

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		worldDraw(&world);

		SDL_GL_SwapBuffers();
		
		
		const int delay=30-SDL_GetTicks()+t;

		if(delay>0)
			SDL_Delay(delay);
			
	}
		
}

