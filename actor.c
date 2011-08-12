
#include "config.h"

#include "actor.h"

#include "world.h"
#include "utils.h"

#include <math.h>

public void actorDrawBBox(Actor* actor)
{

    Vec4f edges[12][2] = {
        {{-1,-1,-1},{-1,1,-1}},
        {{-1,1,-1},{1,1,-1}},
        {{1,1,-1},{1,-1,-1}},
        {{1,-1,-1},{-1,-1,-1}},
        {{-1,-1,1},{-1,1,1}},
        {{-1,1,1},{1,1,1}},
        {{1,1,1},{1,-1,1}},
        {{1,-1,1},{-1,-1,1}},
        {{-1,-1,-1},{-1,-1,1}},
        {{-1,1,-1},{-1,1,1}},
        {{1,-1,-1},{1,-1,1}},
        {{1,1,-1},{1,1,1}},
    };

    glDisable(GL_DEPTH_TEST);

    glBegin(GL_LINES);
    for(int i=0;i<12;i++)
    {
        glColor3f(1,1,1);
        glVertexf(actor->pos+actor->size*edges[i][0]);
        glColor3f(1,1,1);
        glVertexf(actor->pos+actor->size*edges[i][1]);
    }
    glEnd();

}

private bool actorCollision(World* world, const Actor* actor)
{
    for(int z=floor(actor->pos[2]-actor->size[2]+0.01);z<=floor(actor->pos[2]+actor->size[2]-0.01);z++)
    for(int y=floor(actor->pos[1]-actor->size[1]+0.01);y<=floor(actor->pos[1]+actor->size[1]-0.01);y++)
    for(int x=floor(actor->pos[0]-actor->size[0]+0.01);x<=floor(actor->pos[0]+actor->size[0]-0.01);x++)
    {

        Block block=worldGet(world,(Vec4i){x,y,z});

        if(block_definition[block.id].solid)
            return true;
    }

    return false;

}

public void actorTick(World* world, Actor* actor)
{

    for(int d=0;d<3;d++)
    {

        float grid=actor->size[d];
        //float pos0=actor->pos[d];
        float pos1=actor->pos[d]+actor->v[d];
        float s=sign(actor->v[d]);
        while(actor->pos[d]*s<pos1*s)
        {

            float old_pos=actor->pos[d];

            if(actor->v[d]>0)
            {
                actor->pos[d]=min(floor(actor->pos[d]+grid+1)-grid,pos1);
            }
            else
            {
                actor->pos[d]=max(ceil(actor->pos[d]-grid-1)+grid,pos1);
            }

            if(actorCollision(world,actor))
            {
                actor->v[d]=0.0;
                actor->pos[d]=old_pos;
                break;
            }

        }

    }

    actor->v[2]-=0.04;

}

public Pickup* pickupNew()
{
    return calloc(sizeof(Pickup),1);
}

public Mob* mobNew()
{
    return calloc(sizeof(Mob),1);
}

