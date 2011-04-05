
#include "bool.h"
#include "error.h"

#include <SDL.h>
#include <glew.h>

SDL_Surface* screen;
bool fullscreen=false;
SDL_Rect window_rect={0,0,640,480};
SDL_Rect fullscreen_rect;
bool grab_mouse=true;

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

void quit()
{
	SDL_Quit();
	exit(0);
}



void handleEvent(const SDL_Event* event)
{
	switch(event->type)
	{
		case SDL_QUIT:
			quit();
			break;
		case SDL_KEYDOWN:
			switch(event->key.keysym.sym)
			{
				case SDLK_ESCAPE:
					grab_mouse=!grab_mouse;
					fullscreen&=grab_mouse;
					initVideo();
					break;
				case SDLK_F10:
					quit();
					break;
				case SDLK_F11:
					fullscreen=!fullscreen;
					grab_mouse|=fullscreen;
					initVideo();
					break;
				default:
					break;
			}
			break;
		case SDL_VIDEORESIZE:
			window_rect=(SDL_Rect){0,0,event->resize.w,event->resize.h};
			initVideo();
			break;
/*			
		case SDL_MOUSEMOTION:
			if(grab_mouse)
			{
				player.rotX-=event->motion.xrel/100.0;
				player.rotY-=event->motion.yrel/100.0;
				player.rotY=clampf(player.rotY,0,M_PI);
			}
			break;
*/
		default:
			break;
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
		
	while(true)
	{
		
		SDL_Event event;

		while(SDL_PollEvent(&event))
			handleEvent(&event);

		//worldTick(&world);

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//worldDraw(&world);

		SDL_GL_SwapBuffers();
		SDL_Delay(10);
			
	}
		
}

/*

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
			handleEvent(&event);

		worldTick(*world);



		SDL_GL_SwapBuffers();
		SDL_Delay(10);

	}

}
*/
