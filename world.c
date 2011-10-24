
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
        return (Block){};

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

    for(int z=-1;z<=+1;z++)
    for(int y=-1;y<=+1;y++)
    for(int x=-1;x<=+1;x++)
        worldTouch(this,pos+(Vec4i){z,y,x});

    //segment->rendered=false;

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

public void worldScroll(World* world, Vec4f pos)
{

    Vec4i scroll=
    {
        pos[0]/SEGMENT_SIZE-VIEW_RANGE/2,
        pos[1]/SEGMENT_SIZE-VIEW_RANGE/2,
        pos[2]/SEGMENT_SIZE-VIEW_RANGE/2,
    };

    if(scroll[0]!=world->scroll[0] || scroll[1]!=world->scroll[1] || scroll[2]!=world->scroll[2])
    {

        Segment* segment[VIEW_RANGE][VIEW_RANGE][VIEW_RANGE]={};
        Vec4i delta=scroll-world->scroll;

        for(int z=0;z<VIEW_RANGE;z++)
        for(int y=0;y<VIEW_RANGE;y++)
        for(int x=0;x<VIEW_RANGE;x++)
        {

            Segment* s=world->segment[z][y][x];

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

        memcpy(world->segment,segment,sizeof(segment));
        world->scroll=scroll;

    }

}

public void worldTick(World* this,float timeDelta)
{

    this->ticks=this->lastSyncTicks+(SDL_GetTicks()-this->lastSyncTime)*20/1000;

    //playerTick(this);

    for(int i=0;i<this->maxActors;i++)
    {
        Actor* actor=worldGetActor(this,i);

        if(actor!=NULL)
        {
            actorTick(this,actor,timeDelta);
        }

    }

}


public void worldInit(World *this)
{

    *this=(World){};

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

public void worldAddActor(World* world, uint eid, Actor* actor)
{

    assert(world!=NULL && actor!=NULL && eid>0);

    if(world->actor == NULL)
    {
        world->actor=calloc(sizeof(Actor*),1024*32);
        world->maxActors=1024*32;
    }

    assert(eid<world->maxActors);
    //assert(world->actor[eid]==NULL);

    world->actor[eid]=actor;

}

public Actor* worldRemoveActor(World* world, uint eid)
{
    if(world->actor == NULL || eid>world->maxActors)
        panic("unable to remove actor %i\n",eid);

    Actor* actor=world->actor[eid];

    world->actor[eid]=NULL;

    return actor;

}

public Actor* worldGetActor(World* world, uint eid)
{
    if(world->actor == NULL || eid>world->maxActors)
        return NULL;

    return world->actor[eid];
}
