
#include "block.h"
#include "utils.h"
#include "error.h"

#include <SDL.h>
#include <glew.h>
#include <stdlib.h>

Segment segments[16][16][16];
GLuint texture;

void randomizeSegment(Segment* segment)
{
	for(int z=0;z<16;z++)
	for(int y=0;y<16;y++)
	for(int x=0;x<16;x++)
	{
		segment->data[z][y][x]=rand()%2==0;
	}
}

void init()
{
	texture=loadTexture("terrain.png");
	assert(texture!=0);
	randomize();
}

void randomize()
{
	for(int z=0;z<1;z++)
	for(int y=0;y<16;y++)
	for(int x=0;x<16;x++)
	{
		randomizeSegment(&segments[z][y][x]);
	}
}

void draw()
{

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	
	glMatrixMode(GL_MODELVIEW);

	glBindTexture(GL_TEXTURE_2D, texture);

	for(int z=0;z<16;z++)
	for(int y=0;y<16;y++)
	for(int x=0;x<16;x++)
	{
		glPushMatrix();
		glTranslatef(x*16,y*16,z*16);
		drawSegment(&segments[z][y][x]);
		glPopMatrix();
	}


}

void drawSegment(const Segment* segment)
{
	glBegin(GL_QUADS);

	for(int z=0;z<16;z++)
	for(int y=0;y<16;y++)
	for(int x=0;x<16;x++)
	{

		if(segment->data[z][y][x])
		{

			srand((x*16+y)*16+z);
			//glColor3ub(rand()%255,rand()%255,rand()%255);

			int pat[6][4][3]={
				{{0,0,0},{1,0,0},{1,1,0},{0,1,0}},
				{{0,0,1},{0,1,1},{1,1,1},{1,0,1}},

				{{0,0,0},{0,0,1},{1,0,1},{1,0,0}},
				{{0,1,0},{1,1,0},{1,1,1},{0,1,1}},

				{{0,0,0},{0,1,0},{0,1,1},{0,0,1}},
				{{1,0,0},{1,0,1},{1,1,1},{1,1,0}},
			};

			int texCoord[][2]={
				{0,0},{0,1},{1,1},{1,0},
			};

			for(int i=0;i<6;i++)
			for(int v=0;v<4;v++)
			{
				glTexCoord2f((texCoord[v][0]+1)*1.0/16.0,texCoord[v][1]*1.0/16.0);
				glVertex3i(x+pat[i][v][0],y+pat[i][v][1],  z+pat[i][v][2]);
			}

		}
	}

	glEnd();

}

Segment* loadSegment(int x, int y, int z)
{
	return NULL;
}

void worldTick()
{
	Uint8 *keys = SDL_GetKeyState(NULL);

	double vx=0,vy=0;


	if(keys[SDLK_w])
	{
		vy+=1;
	}
	if(keys[SDLK_s])
	{
		vy-=1;
	}
	if(keys[SDLK_a])
	{
		vx-=1;
	}
	if(keys[SDLK_d])
	{
		vx+=1;
	}

	player.y-=cos(player.rotX)*sin(player.rotY)*vy*0.05;
	player.x-=sin(player.rotX)*sin(player.rotY)*vy*0.05;
	player.z-=cos(player.rotY)*vy*0.05;

	player.y-=sin(player.rotX)*vx*0.05;
	player.x-=-cos(player.rotX)*vx*0.05;
}

void worldDraw()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glRotatef(player.rotY*180/M_PI,1,0,0);
	glRotatef(player.rotX*180/M_PI,0,0,1);

	glTranslatef(-player.x,-player.y,-player.z);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//		draw();

	glCallList(list);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}
