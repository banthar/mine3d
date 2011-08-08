
#include "config.h"

#include "worldgen.h"

#include "world.h"

#include <math.h>

public void prepareSegment(World* world, Vec4i pos)
{

    Vec4i global=pos-world->scroll;

    for(int i=0;i<3;i++)
        if(global[i]<0 || global[i]>=VIEW_RANGE)
            return;

    Segment* segment=world->segment[global[2]][global[1]][global[0]];

    if(segment==NULL)
    {
        segment=newSegment();
        world->segment[global[2]][global[1]][global[0]]=segment;
        generateSegment(world,segment,pos);
    }
}

public void generateSegment(World* world, Segment* segment, Vec4i pos)
{

    segment->empty=true;

    for(int y=0;y<SEGMENT_SIZE;y++)
    for(int x=0;x<SEGMENT_SIZE;x++)
    {

        Vec2f xy=(Vec2f){x+pos[0]*SEGMENT_SIZE,y+pos[1]*SEGMENT_SIZE};

        float stoneHeight=64;
        stoneHeight+=noise2(&world->noise,xy*(Vec2f){0.001,0.001})*75;
        stoneHeight+=noise2(&world->noise,xy*(Vec2f){0.01,0.01})*50;
        stoneHeight+=noise2(&world->noise,xy*(Vec2f){0.1,0.1})*5;

        if(stoneHeight<0)
            stoneHeight=-sqrt(-stoneHeight)*1.5;

        float dirtHeight=64+noise2(&world->noise,xy*(Vec2f){0.01,0.01})*10-1;
        dirtHeight+=noise2(&world->noise,xy*(Vec2f){0.1,0.1});

        float snowHeight=64+noise2(&world->noise,xy*(Vec2f){0.01,0.01})*10+35;
        snowHeight+=noise2(&world->noise,xy*(Vec2f){0.1,0.1})*5;


        for(int z=0;z<SEGMENT_SIZE;z++)
        {

            Vec4f xyz=(Vec4f){x+pos[0]*SEGMENT_SIZE,y+pos[1]*SEGMENT_SIZE,z+pos[2]*SEGMENT_SIZE};

            if(xyz[2]<=0)
            {
                segment->data[z][y][x].id=7;
            }
            else if(stoneHeight>xyz[2])
            {
                segment->data[z][y][x].id=1;
            }
            else if(stoneHeight>xyz[2]-(xyz[2]-snowHeight)/8 && snowHeight<xyz[2])
            {
                segment->data[z][y][x].id=78;
            }
            else if(dirtHeight>xyz[2])
            {
                segment->data[z][y][x].id=2;
            }
            else
            {
                segment->data[z][y][x].id=0;
            }

            if(segment->data[z][y][x].id!=0)
                segment->empty=false;

        }

    }

}

