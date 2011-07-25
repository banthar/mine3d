
#include "config.h"

#include "worldgen.h"

#include "world.h"

#include <math.h>

public void generateSegment(World* world, Segment* segment, Vec4i pos)
{

	segment->empty=true;
	
	for(int y=0;y<SEGMENT_SIZE;y++)
	for(int x=0;x<SEGMENT_SIZE;x++)
	{
		
		Vec2f xy=(Vec2f){x+pos[0]*SEGMENT_SIZE,y+pos[1]*SEGMENT_SIZE};

		float stoneHeight=0.0;
		stoneHeight+=noise2(&world->noise,xy*(Vec2f){0.001,0.001})*100;
		stoneHeight+=noise2(&world->noise,xy*(Vec2f){0.01,0.01})*50;
		stoneHeight+=noise2(&world->noise,xy*(Vec2f){0.1,0.1})*5;

		if(stoneHeight<0)
			stoneHeight=-sqrt(-stoneHeight)*1.5;

		float dirtHeight=noise2(&world->noise,xy*(Vec2f){0.01,0.01})*10-1;
		dirtHeight+=noise2(&world->noise,xy*(Vec2f){0.1,0.1});

		float snowHeight=noise2(&world->noise,xy*(Vec2f){0.01,0.01})*10+50;
		snowHeight+=noise2(&world->noise,xy*(Vec2f){0.1,0.1})*5;


		for(int z=0;z<SEGMENT_SIZE;z++)
		{

			Vec4f xyz=(Vec4f){x+pos[0]*SEGMENT_SIZE,y+pos[1]*SEGMENT_SIZE,z+pos[2]*SEGMENT_SIZE};

			if(stoneHeight>xyz[2])
			{
				segment->data[z][y][x].id=2;
			}
			else if(stoneHeight>xyz[2]-(xyz[2]-snowHeight)/8 && snowHeight<xyz[2])
			{
				segment->data[z][y][x].id=3;
			}
			else if(dirtHeight>xyz[2])
			{
				segment->data[z][y][x].id=1;
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

