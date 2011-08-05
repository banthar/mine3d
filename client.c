
#include "config.h"

#include "client.h"

#include "world.h"
#include "utils.h"
#include "network.h"

#include <signal.h>
#include "SDL.h"
#include "glew.h"

private void grabMouse(Client* client)
{
	SDL_ShowCursor(!client->grab_mouse);
	SDL_WM_GrabInput(client->grab_mouse?SDL_GRAB_ON:SDL_GRAB_OFF);	
}

private void initVideo(Client* client)
{
	if(client->fullscreen)
		client->screen=SDL_SetVideoMode(client->fullscreen_rect.w,client->fullscreen_rect.h,0,SDL_OPENGL|SDL_FULLSCREEN);
	else
		client->screen=SDL_SetVideoMode(client->window_rect.w,client->window_rect.h,0,SDL_OPENGL|SDL_RESIZABLE);

	if(client->screen==NULL)
		panic("unable to set video mode");

	grabMouse(client);

	glViewport(0,0,client->screen->w,client->screen->h);

	float aspect=(float)client->screen->w/(float)client->screen->h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float d=0.1;

	glFrustum(-aspect*d,aspect*d,1*d,-1*d,0.1,300);

}

private __attribute__((noreturn)) void quit(Client* client)
{
	worldDestroy(&client->world);
	SDL_Quit();
	exit(0);
}


private bool startDigginig(Client* client)
{

	Vec4i p=worldRay(&client->world, client->world.player.pos+client->world.player.headOffset,  rotationNormal(client->world.player.rot),3);

	if(p[3]==-1)
		return false;

	sendPlayerDigging(client,0,p,p[3]);
	sendPlayerDigging(client,2,p,p[3]);
	
	worldSet(&client->world,p,(Block){.id=0});
	
	return true;
}

private void saveScreenshoot(Client* client)
{
	
	byte pixels[client->screen->w*client->screen->h*3];
	
	glReadPixels(0,0,client->screen->w,client->screen->h,GL_RGB,GL_UNSIGNED_BYTE,pixels);

	FILE* out=fopen("screen.ppm","w");
	fprintf(out,"P6\n%i %i\n255\n",client->screen->w,client->screen->h);
	fwrite(pixels,sizeof(pixels),1,out);
	fclose(out);

}

private bool handleEvent(Client* client, const SDL_Event* event)
{
	switch(event->type)
	{
		case SDL_QUIT:
			quit(client);
		case SDL_KEYDOWN:
			switch(event->key.keysym.sym)
			{
				case SDLK_ESCAPE:
					client->grab_mouse=!client->grab_mouse;
					client->fullscreen&=client->grab_mouse;
					initVideo(client);
					return false;
				case SDLK_F10:
					quit(client);
				case SDLK_F11:
					client->fullscreen=!client->fullscreen;
					client->grab_mouse|=client->fullscreen;
					initVideo(client);
					return false;
				case SDLK_F2:
					saveScreenshoot(client);
					return false;
				default:
					return true;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch(event->button.button)
			{
				case 1:
					return startDigginig(client);
				default:
					return false;
			}
		case SDL_MOUSEMOTION:
			if(client->grab_mouse || event->motion.state)
				return true;
			else
				return false;
		case SDL_VIDEORESIZE:
			client->window_rect=(SDL_Rect){0,0,event->resize.w,event->resize.h};
			initVideo(client);
			return false;
		default:
			return true;
	}
}

export int main(int argc, char* argv[])
{

	if(SDL_Init(SDL_INIT_VIDEO)!=0)
		panic("sdl error");

	//signal(SIGINT, SIG_DFL);

	Client client={
		.window_rect=(SDL_Rect){0,0,720,480},
		.worldLock=SDL_CreateMutex(),
		.socketLock=SDL_CreateMutex(),
	};

	const SDL_VideoInfo* video_info=SDL_GetVideoInfo();
	client.fullscreen_rect=(SDL_Rect){0,0,video_info->current_w,video_info->current_h};

	initVideo(&client);

	if(glewInit()!=GLEW_OK)
		panic("glew error");

	GLint screen_texture=emptyTexture(1,1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	worldInit(&client.world);
	SDL_LockMutex(client.worldLock);
	worldDisplayInit(&client.world);

	SDL_CreateThread(networkMain, &client);

	while(!client.stop)
	{
		
		SDL_Event event;

		while(SDL_PollEvent(&event))
		{
			if(handleEvent(&client, &event))
				worldEvent(&client.world,&event);
		}

		int t=SDL_GetTicks();

		//worldTick(&client.world);

		glClearColor(0.76,0.81,1.0,0.0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		worldDraw(&client.world);

		SDL_UnlockMutex(client.worldLock);

		glBindTexture(GL_TEXTURE_2D,screen_texture);
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, client.screen->w, client.screen->h, 0);
		glGenerateMipmap(GL_TEXTURE_2D);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		//glViewport(0,0,screen->w,screen->h);
		//glScalef(1.01,1.01,1.01);
		//glRotatef(1,0,0,1);
		glBegin(GL_QUADS);
		glTexCoord2f(0,0); glColor4f(1.0,1.0,1.0,1.0); glVertex2f(-1,-1);
		glTexCoord2f(1,0); glColor4f(1.0,1.0,1.0,1.0); glVertex2f( 1,-1);
		glTexCoord2f(1,1); glColor4f(1.0,1.0,1.0,1.0); glVertex2f( 1, 1);
		glTexCoord2f(0,1); glColor4f(1.0,1.0,1.1,1.0); glVertex2f(-1, 1);
		glEnd();
		glPopMatrix();
		
		//glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, screen->w, screen->h, 0);
		//glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,0);
		
		SDL_GL_SwapBuffers();
		
		assert(glGetError()==0);
		
		const int delay=30-SDL_GetTicks()+t;

		if(delay>0)
			SDL_Delay(delay);

		SDL_LockMutex(client.worldLock);
			
	}
		
}

