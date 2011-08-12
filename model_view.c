
#include "config.h"

#include "SDL.h"
#include "glew.h"

#include "model.h"
#include "utils.h"

#include <stdbool.h>
#include <SDL.h>
#include <glew.h>
#include <math.h>

GLuint texture;

Box model={
    {-4, -2, -6},
    {8, 4, 12},
    {16, 16},
    5,
    {
        {{4, 2, 12},&(Box){{-4,-4,0},{8,8,8},{0,0}}},

        {{-2, 2, 10},&(Box){{-2,-2,-10},{4,4,12},{40,16}}},
        {{10, 2, 10},&(Box){{-2,-2,-10},{4,4,12},{40,16}}},

        {{2, 2, 0},&(Box){{-2,-2,-12},{4,4,12},{0,16}}},
        {{6, 2, 0},&(Box){{-2,-2,-12},{4,4,12},{0,16}}},
    }
};

void draw()
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D,texture);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glScalef(0.1,0.1,0.1);
    drawBox(&model);
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D,0);
}


void init()
{
    texture=loadTexture("data/mob/zombie.png");
}


static SDL_Surface* screen;
static bool fullscreen=false;
static SDL_Rect window_rect={0,0,720,420};
static SDL_Rect fullscreen_rect;
static bool grab_mouse=false;

static float rotX=0.0;
static float rotY=1.0;
static float zoom=2.0;

static void grabMouse()
{
    SDL_ShowCursor(!grab_mouse);
    SDL_WM_GrabInput(grab_mouse?SDL_GRAB_ON:SDL_GRAB_OFF);
}

static void initVideo()
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

__attribute__((noreturn)) static void quit()
{
    SDL_Quit();
    exit(0);
}

static bool handleEvent(const SDL_Event* event)
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
        case SDL_MOUSEBUTTONDOWN:
            if(event->button.button==4)
            {
                zoom*=0.9;
            }
            else if(event->button.button==5)
            {
                zoom/=0.9;
            }
            return false;
        case SDL_MOUSEMOTION:
            if(grab_mouse||event->motion.state)
            {
                rotX-=event->motion.xrel/100.0;
                rotY-=event->motion.yrel/100.0;
            }
            return false;
        default:
            return true;
    }
}

static void drawScene()
{

    glClearColor(0.5,0.5,0.5,0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    //glRotatef(0,0,1,rotX);
    //glRotatef(0,1,0,rotY);
    glTranslatef(0,0,-zoom);
    glRotatef(rotY*180/M_PI,1,0,0);
    glRotatef(rotX*180/M_PI,0,0,1);

    glColor3f(0.75,0.75,0.75);

    glBegin(GL_LINES);

    for(int i=-10;i<11;i++)
    {
        glVertex3f(  -1.0,i/10.0,0.0);
        glVertex3f(   1.0,i/10.0,0.0);
        glVertex3f(i/10.0,  -1.0,0.0);
        glVertex3f(i/10.0,   1.0,0.0);
    }

    glEnd();

    glColor3f(1,1,1);

    draw();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

}

int main(int argc, char* argv)
{

    SDL_Init(SDL_INIT_VIDEO);

    const SDL_VideoInfo* video_info=SDL_GetVideoInfo();
    fullscreen_rect=(SDL_Rect){0,0,video_info->current_w,video_info->current_h};

    initVideo();

    if(glewInit()!=GLEW_OK)
        panic("glew error");

    init();

    while(true)
    {

        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            handleEvent(&event);
        }

        int t=SDL_GetTicks();

        drawScene();

        assert(glGetError()==0);

        SDL_GL_SwapBuffers();

        const int delay=30-SDL_GetTicks()+t;

        if(delay>0)
            SDL_Delay(delay);

    }
}
