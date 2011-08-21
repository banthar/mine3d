
#include "config.h"

#include "client.h"

#include "world.h"
#include "worldgfx.h"
#include "utils.h"
#include "network.h"
#include "player.h"
#include "gui.h"

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

    glScalef(1.0/aspect,1,1);

}

private __attribute__((noreturn)) void quit(Client* client)
{
    worldDestroy(&client->world);
    SDL_Quit();
    exit(0);
}


private bool startDigging(Client* client)
{

    Vec4i p=worldRay(&client->world, client->player->pos+client->player->headOffset,  rotationNormal(client->player->rot),3);

    if(p[3]==-1)
        return false;
    p[3]=1;

    client->player->digging.on=true;
    client->player->digging.start=client->time;
    client->player->digging.location=p;

    sendPlayerDigging(client,0,p);

    return true;
}

private bool abortDigging(Client* client)
{

    client->player->digging.on=false;

    return true;
}

private void tickDigging(Client* client)
{

    if(!client->player->digging.on)
        return;

    Vec4i location=client->player->digging.location;

    if(client->time-client->player->digging.lastAnimation>250)
    {
        sendAnimation(client,client->eid,1);
        client->player->digging.lastAnimation=client->time;
    }

    if(client->time-client->player->digging.start>1100)
    {

        sendPlayerDigging(client,2,location);

        worldSet(&client->world,location,(Block){.id=0});

        client->player->digging.on=false;

        startDigging(client);

    }

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

private void drawGUI(Client* client)
{

    glDisable(GL_DEPTH_TEST);

    glColor3f(1,1,1);

    glBegin(GL_LINES);
    glVertex3f(+1,0,-20);
    glVertex3f(-1,0,-20);

    glVertex3f(0,+1,-20);
    glVertex3f(0,-1,-20);
    glEnd();

}

private bool gameEvent(Client* client, const SDL_Event* event)
{
    switch(event->type)
    {
        case SDL_KEYDOWN:
            switch(event->key.keysym.sym)
            {
                case SDLK_e:
                    client->paused=!client->paused;
                    client->equipmentVisible=!client->equipmentVisible;
                    return true;
                default:
                    return false;
            }
    }

    return false;

}

public bool worldEvent(Client* client, const SDL_Event* event)
{

    switch(event->type)
    {
        case SDL_MOUSEBUTTONDOWN:
            switch(event->button.button)
            {
                case 1:
                    return startDigging(client);
                default:
                    return false;
            }
        case SDL_MOUSEBUTTONUP:
            switch(event->button.button)
            {
                case 1:
                    return abortDigging(client);
                default:
                    return false;
            }
        case SDL_KEYDOWN:
            switch(event->key.keysym.sym)
            {
                case SDLK_f:
                    client->player->flying=!client->player->flying;
                    return true;
                case SDLK_SPACE:
                    client->player->v[2]=6.5;
                    return true;
                default:
                    return false;
            }
        case SDL_MOUSEMOTION:
            if(client->grab_mouse)
            {
                client->player->rot[0]+=event->motion.xrel;
                client->player->rot[1]-=event->motion.yrel;
                client->player->rot[1]=clampf(client->player->rot[1],0,180);
            }
            return false;
        default:
            return true;
    }
}


private void playerTick(Client* client)
{

    Player* player=client->player;

    double vx=0,vy=0;
    double v=4.27;

    if(!client->paused)
    {

        Uint8 *keys = SDL_GetKeyState(NULL);

        if(keys[SDLK_w] || keys[SDLK_UP])
        {
            vy+=1;
        }
        if(keys[SDLK_s] || keys[SDLK_DOWN])
        {
            vy-=1;
        }
        if(keys[SDLK_a] || keys[SDLK_LEFT])
        {
            vx-=1;
        }
        if(keys[SDLK_d] || keys[SDLK_RIGHT])
        {
            vx+=1;
        }
        if(keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT])
        {
            v=0.1;
        }

    }

    if(!player->flying)
    {
        player->v[0]=-cos(player->rot[0]/180.0*M_PI)*vx*v-sin(player->rot[0]/180.0*M_PI)*vy*v;
        player->v[1]=-sin(player->rot[0]/180.0*M_PI)*vx*v+cos(player->rot[0]/180.0*M_PI)*vy*v;

        //actorTick(world,player);
    }
    else
    {

        player->v[0]=-sin(player->rot[0]/180.0*M_PI)*sin(player->rot[1]/180.0*M_PI)*vy*v-cos(player->rot[0]/180.0*M_PI)*vx*v;
        player->v[1]=cos(player->rot[0]/180.0*M_PI)*sin(player->rot[1]/180.0*M_PI)*vy*v-sin(player->rot[0]/180.0*M_PI)*vx*v;
        player->v[2]=-cos(player->rot[1]/180.0*M_PI)*vy*v;

        //actorTick(world,player);

    }

}

private bool mainEvent(Client* client, const SDL_Event* event)
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
        case SDL_VIDEORESIZE:
            client->window_rect=(SDL_Rect){0,0,event->resize.w,event->resize.h};
            initVideo(client);
            return false;
        default:
            return true;
    }
}

private void clientDraw(Client* client)
{

    World* world=&client->world;

    float timeDelta=(SDL_GetTicks()-world->drawStart)/1000.0;

    world->drawStart=SDL_GetTicks();


    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    //glTranslated(0,0,-3);
    float d=0.1;
    glFrustum(-d,d,d,-d,0.1,300);


    glRotatef(client->player->rot[1],1,0,0);
    glRotatef(180.0-client->player->rot[0],0,0,1);

    glTranslatev(-client->player->pos-client->player->headOffset);

    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINES);
    int n=16;
    for(int i=-n;i<n;i++)
    {
        glVertex3f(-16*n,i*16,0);
        glVertex3f(16*n,i*16,0);
        glVertex3f(i*16,-16*n,0);
        glVertex3f(i*16,16*n,0);
    }
    glEnd();

    //actorDrawBBox(&world->player);

    worldTick(world,timeDelta);

    //float daytime=(world->ticks%24000ull)/24000.0;

    worldDraw(&client->world);

    Vec4i p=worldRay(world, client->player->pos+client->player->headOffset,  rotationNormal(client->player->rot),3);

    if(p[3]!=-1)
        worldDrawSelection(world,p);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

}

export int main(int argc, char* argv[])
{

    if(SDL_Init(SDL_INIT_VIDEO)!=0)
        panic("sdl error");

    //signal(SIGINT, SIG_DFL);

    Client client={
        .window_rect=(SDL_Rect){0,0,854,480},
        .worldLock=SDL_CreateMutex(),
        .socketLock=SDL_CreateMutex(),
        .playerName="Player56",
        .player=playerNew(),


    };

    client.player->draw=false;

    const SDL_VideoInfo* video_info=SDL_GetVideoInfo();
    client.fullscreen_rect=(SDL_Rect){0,0,video_info->current_w,video_info->current_h};

    initVideo(&client);

    if(glewInit()!=GLEW_OK)
        panic("glew error");

    //GLint screen_texture=emptyTexture(1,1);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    client.zombieTexture=loadTexture("data/mob/char.png");

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    worldInit(&client.world);
    SDL_LockMutex(client.worldLock);
    worldDisplayInit(&client.world);

    SDL_CreateThread(networkMain, &client);

    while(!client.stop)
    {

        client.time=SDL_GetTicks();

        SDL_Event event;

        while(SDL_PollEvent(&event))
        {

            mainEvent(&client, &event);

            gameEvent(&client,&event);

            //guiEvent(&client,&event);

            if(!client.paused)
                worldEvent(&client,&event);

        }


        //worldTick(&client.world);

        glClearColor(0.76,0.81,1.0,0.0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


        playerTick(&client);

        tickDigging(&client);

        clientDraw(&client);

        //guiDraw(&client);

        SDL_UnlockMutex(client.worldLock);



/*
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
*/
        glBindTexture(GL_TEXTURE_2D,0);

        SDL_GL_SwapBuffers();

        assert(glGetError()==0);

        const int delay=30-SDL_GetTicks()+client.time;

        if(delay>0)
            SDL_Delay(delay);

        SDL_LockMutex(client.worldLock);

    }

}

