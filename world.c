
#include "config.h"

#include "world.h"

#include "utils.h"
#include "noise.h"
#include "block.h"
#include "utils.h"

#include "SDL.h"
#include "glew.h"
#include <math.h>
#include <stdlib.h>
#include <pthread.h>

public Block segmentGet(Segment* this, int x, int y, int z)
{
    if(x<0 || y<0 || z<0 || x>=SEGMENT_SIZE || y>=SEGMENT_SIZE || z>=SEGMENT_SIZE)
        return (Block){};
    else
        return this->data[z][y][x];
}

public Block worldGet(World* this, Vec4i pos)
{

    const Vec4i segment_bits=(Vec4i){SEGMENT_BITS,SEGMENT_BITS,SEGMENT_BITS,SEGMENT_BITS};
    const Vec4i segment_mask=(Vec4i){SEGMENT_MASK,SEGMENT_MASK,SEGMENT_MASK,SEGMENT_MASK};

    Vec4i global=(pos>>segment_bits)-this->scroll;

    for(int i=0;i<3;i++)
        if(global[i]<0 || global[i]>=VIEW_RANGE)
            return (Block){.skyLight=1.0};

    Segment* segment=this->segment[global[2]][global[1]][global[0]];

    if(segment==NULL)
        return (Block){.skyLight=1.0};

    Vec4i local=pos&segment_mask;

    return segment->data[local[2]][local[1]][local[0]];

}

private void worldTouch(World* this, Vec4i pos)
{

    const Vec4i segment_bits=(Vec4i){SEGMENT_BITS,SEGMENT_BITS,SEGMENT_BITS,SEGMENT_BITS};

    Vec4i global=(pos>>segment_bits)-this->scroll;

    for(int i=0;i<3;i++)
        if(global[i]<0 || global[i]>=VIEW_RANGE)
            return;

    Segment* segment=this->segment[global[2]][global[1]][global[0]];

    if(segment!=NULL)
    {
        segment->rendered=false;
    }

}

public void worldSet(World* this, Vec4i pos, Block block)
{

    const Vec4i segment_bits=(Vec4i){SEGMENT_BITS,SEGMENT_BITS,SEGMENT_BITS,SEGMENT_BITS};
    const Vec4i segment_mask=(Vec4i){SEGMENT_MASK,SEGMENT_MASK,SEGMENT_MASK,SEGMENT_MASK};

    Vec4i global=(pos>>segment_bits)-this->scroll;

    for(int i=0;i<3;i++)
        if(global[i]<0 || global[i]>=VIEW_RANGE)
            return;

    Segment* segment=this->segment[global[2]][global[1]][global[0]];

    if(segment==NULL)
    {
        segment=newSegment();
        this->segment[global[2]][global[1]][global[0]]=segment;
    }

    worldTouch(this,pos+(Vec4i){0,0, 1});
    worldTouch(this,pos+(Vec4i){0,0,-1});
    worldTouch(this,pos+(Vec4i){ 1,0,0});
    worldTouch(this,pos+(Vec4i){-1,0,0});
    worldTouch(this,pos+(Vec4i){0, 1,0});
    worldTouch(this,pos+(Vec4i){0,-1,0});

    segment->rendered=false;

    if(block.id!=0)
        segment->empty=false;

    Vec4i local=pos&segment_mask;

    segment->data[local[2]][local[1]][local[0]]=block;

}

public Vec4i worldRay(World* world, Vec4f pos0, Vec4f normal, int max_length)
{

    Vec4i pos1=(Vec4i){floor(pos0[0]),floor(pos0[1]),floor(pos0[2])};
    Vec4i step=(Vec4i){sign(normal[0]),sign(normal[1]),sign(normal[2])};
    Vec4f delta=(Vec4f){fabs(1/normal[0]),fabs(1/normal[1]),fabs(1/normal[2])};
    Vec4f max;

    for(int d=0;d<3;d++)
    {

        if(normal[d]==0)
            max[d]=1.0/0.0;
        else if(normal[d]<0)
            max[d]=-fract(pos0[d])/normal[d];
        else
            max[d]=(1-fract(pos0[d]))/normal[d];

    }

    while(true)
    {

        int i;

        if(max[0]<=max[1] && max[0]<=max[2])
            i=0;
        else if(max[1]<=max[0] && max[1]<=max[2])
            i=1;
        else if(max[2]<=max[0] && max[2]<=max[1])
            i=2;
        else
            panic("unreachable");

        pos1[i]+=step[i];
        max[i]+=delta[i];

        if(abs(pos1[i]-pos0[i])>max_length)
            break;

        if(worldGet(world,pos1).id!=0)
        {
            int face=(i<<1)|(step[i]<0);
            pos1[3]=face;
            return pos1;
        }
    }

    return (Vec4i){0,0,0,-1};

}

static int allocated_segments=0;

public Segment* newSegment()
{
    allocated_segments++;
    return calloc(1,sizeof(Segment));
}

private void freeSegment(Segment* this)
{

    if(this==NULL)
        return;

    allocated_segments--;

    if(this->vbo!=0)
        glDeleteBuffers(1,&this->vbo);

    free(this);

}

public void worldSpiral(World* this, void (f)(World*,int x,int y,int z))
{

    int c=VIEW_RANGE/2;

    for(int r=0;r<c;r++)
    for(int y=-r;y<r;y++)
    for(int x=-r;x<r;x++)
    {
        if(y==-r || x==-r || y==r-1 || x==r-1)
        {
            for(int z=-r;z<r;z++)
                f(this,c+x,c+y,c+z);
        }
        else
        {
            f(this,c+x,c+y,c+r-1);
            f(this,c+x,c+y,c-r);
        }
    }

}

public bool worldEvent(World* this, const SDL_Event* event)
{

    switch(event->type)
    {
        case SDL_KEYDOWN:
            switch(event->key.keysym.sym)
            {
                case SDLK_f:
                    this->player.flying=!this->player.flying;
                    return true;
                case SDLK_SPACE:
                    this->player.v[2]=0.31;
                    return true;
                default:
                    return false;
            }
        case SDL_MOUSEMOTION:
            this->player.rot[0]-=event->motion.xrel/100.0;
            this->player.rot[1]-=event->motion.yrel/100.0;
            this->player.rot[1]=clampf(this->player.rot[1],0,M_PI);
            return false;
        default:
            return true;
    }
}

private void playerTick(World* world)
{
    //int t=SDL_GetTicks();
    Uint8 *keys = SDL_GetKeyState(NULL);

    double vx=0,vy=0;
    double v=0.15;

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
        //v=0.01*world->player.pos[0];
        v=0.1;
    }

    if(!world->player.flying)
    {
        world->player.v[0]=cos(world->player.rot[0])*vx*v-sin(world->player.rot[0])*vy*v;
        world->player.v[1]=-sin(world->player.rot[0])*vx*v-cos(world->player.rot[0])*vy*v;

        actorTick(world,&world->player);
    }
    else
    {

        world->player.v[0]=-sin(world->player.rot[0])*sin(world->player.rot[1])*vy*v+cos(world->player.rot[0])*vx*v;
        world->player.v[1]=-cos(world->player.rot[0])*sin(world->player.rot[1])*vy*v-sin(world->player.rot[0])*vx*v;
        world->player.v[2]=-cos(world->player.rot[1])*vy*v;

        actorTick(world,&world->player);

    }

}

public void worldTick(World* this)
{

    this->ticks=this->lastSyncTicks+(SDL_GetTicks()-this->lastSyncTime)*20/1000;

    playerTick(this);


    Vec4i scroll=
    {
        this->player.pos[0]/SEGMENT_SIZE-VIEW_RANGE/2,
        this->player.pos[1]/SEGMENT_SIZE-VIEW_RANGE/2,
        this->player.pos[2]/SEGMENT_SIZE-VIEW_RANGE/2,
    };

    if(scroll[0]!=this->scroll[0] || scroll[1]!=this->scroll[1] || scroll[2]!=this->scroll[2])
    {

        Segment* segment[VIEW_RANGE][VIEW_RANGE][VIEW_RANGE]={};
        Vec4i delta=scroll-this->scroll;

        for(int z=0;z<VIEW_RANGE;z++)
        for(int y=0;y<VIEW_RANGE;y++)
        for(int x=0;x<VIEW_RANGE;x++)
        {

            Segment* s=this->segment[z][y][x];

            int ix=x-delta[0];
            int iy=y-delta[1];
            int iz=z-delta[2];

            if(ix<0 || iy<0 || iz<0 || ix>=VIEW_RANGE || iy>=VIEW_RANGE || iz>=VIEW_RANGE)
            {
                freeSegment(s);
            }
            else
            {
                segment[iz][iy][ix]=s;
            }

        }

        memcpy(this->segment,segment,sizeof(segment));
        this->scroll=scroll;

    }

    //if(this->socket!=NULL)
    //  sendPlayerPositionAndLook(this, this->socket);

    //printf("ticks: %i ",SDL_GetTicks()-t);
    //printf("segments: %i ",allocated_segments);
    //printf("player: %f %f %f\n",this->player.pos[0],this->player.pos[1],this->player.pos[2]);

}


public void worldInit(World *this)
{

    *this=(World){};

    this->player=(Player){.size={0.3,0.3,0.9},.headOffset={0.0,0.0,0.89}};

    this->scroll=(Vec4i){-VIEW_RANGE/2,-VIEW_RANGE/2,0};

    noiseInit(&this->noise,666);

}

public void worldDestroy(World* this)
{

    glDeleteTextures(1,&this->terrain);

    for(int z=0;z<VIEW_RANGE;z++)
    for(int y=0;y<VIEW_RANGE;y++)
    for(int x=0;x<VIEW_RANGE;x++)
    {
        freeSegment(this->segment[z][y][x]);
    }

}

