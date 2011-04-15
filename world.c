
#include "world.h"
#include "utils.h"
#include "error.h"
#include "bool.h"

#include "bool.h"

#include <math.h>
#include <SDL.h>
#include <glew.h>
#include <stdlib.h>
#include <pthread.h>

void init();
void randomize();
void draw();
void drawSegment(Segment* this);
Segment* loadSegment(int x, int y, int z);

unsigned int noise3D(Vec4i v)
{
	unsigned int s=0xcafebabeu;
	
	s^=v[0]^v[1]^v[2];
	
	for(int i=0;i<8;i++)
	{
		s+=(unsigned int)v[s%4];
		s=s*2971215073u;
		s+=11u;
		s=(s>>16u)^(s<<16u);
	}
	
	
	return s;
}

int perlin3D(Vec4i v)
{

	int s=0;
	
	for(int i=0;i<8;i++)
	{
		int n=1<<i;
		s+=noise3D(v/(Vec4i){n,n,n,n})/256;
		s/=2;
	}
	
	return s/256/256;

}

void generateSegment(Segment* this, Vec4i pos)
{
	for(int z=0;z<SEGMENT_SIZE;z++)
	for(int y=0;y<SEGMENT_SIZE;y++)
	for(int x=0;x<SEGMENT_SIZE;x++)
	{
		
		Vec4i l=pos*(Vec4i){SEGMENT_SIZE,SEGMENT_SIZE,SEGMENT_SIZE}+(Vec4i){x,y,z};
		
		this->data[z][y][x]=noise3D(l)%1000==0;
			
		
		
	}
	
}

void drawSegmentVBO(Segment* this)
{
	assert(this!=NULL);

}

void drawSegment(Segment* this)
{

	assert(this!=NULL);
	
	if(this->list==0)
	{

		this->list=glGenLists(1);
		glNewList(this->list, GL_COMPILE);
		
		glBegin(GL_QUADS);

		for(int z=0;z<SEGMENT_SIZE;z++)
		for(int y=0;y<SEGMENT_SIZE;y++)
		for(int x=0;x<SEGMENT_SIZE;x++)
		{

			if(this->data[z][y][x]!=0)
			{

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

				int tileX=this->data[z][y][x]%TEXTURE_SIZE;
				int tileY=this->data[z][y][x]/TEXTURE_SIZE;

				//glColor3f(this->data[z][y][x]/16.0/16.0,this->data[z][y][x]/16.0/16.0,this->data[z][y][x]/16.0/16.0);

				for(int i=0;i<6;i++)
				for(int v=0;v<4;v++)
				{
					glTexCoord2f((texCoord[v][0]+tileX)*1.0/TEXTURE_SIZE,(texCoord[v][1]+tileY)*1.0/TEXTURE_SIZE);
					glVertex3i(x+pat[i][v][0],y+pat[i][v][1],  z+pat[i][v][2]);
				}

			}
		}

		glEnd();
		glEndList();
	}

	glCallList(this->list);

}

Segment* loadSegment(int x, int y, int z)
{
	return NULL;
}

void freeSegment(Segment* this)
{
	
	if(this==NULL)
		return;
	
	if(this->list!=0)
		glDeleteLists(this->list,1);
		 
	free(this);
	
}

void worldInit(World *this)
{
	
	*this=(World){};
	
	this->player.pos[2]=64.0;
	
	this->terrain=loadTexture("terrain.png");
	assert(this->terrain!=0);


}

bool worldEvent(World* this, const SDL_Event* event)
{
	
	switch(event->type)
	{	
		case SDL_MOUSEMOTION:
			//if(grab_mouse)
			{
				this->player.rot[0]-=event->motion.xrel/100.0;
				this->player.rot[1]-=event->motion.yrel/100.0;
				this->player.rot[1]=clampf(this->player.rot[1],0,M_PI);
			}
			return false;
		default:
			return true;
	}
}

void worldTick(World* this)
{
	
	Uint8 *keys = SDL_GetKeyState(NULL);

	double vx=0,vy=0;
	double v=0.5;
	
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

	this->player.pos[0]-=sin(this->player.rot[0])*sin(this->player.rot[1])*vy*v;
	this->player.pos[1]-=cos(this->player.rot[0])*sin(this->player.rot[1])*vy*v;
	this->player.pos[2]-=cos(this->player.rot[1])*vy*v;

	this->player.pos[0]-=-cos(this->player.rot[0])*vx*v;
	this->player.pos[1]-=sin(this->player.rot[0])*vx*v;
	

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
			
			if(ix<0 || iy<0 || iz<0 || ix>=16 || iy>=16 || iz>=16)
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

	int n=10;

	for(int z=0;z<VIEW_RANGE;z++)
	for(int y=0;y<VIEW_RANGE;y++)
	for(int x=0;x<VIEW_RANGE;x++)
	{
		if(this->segment[z][y][x]==NULL)
		{
			this->segment[z][y][x]=calloc(1,sizeof(Segment));
			generateSegment(this->segment[z][y][x],(Vec4i){x,y,z}+this->scroll);
			
			if(n--<0)
				x=y=z=VIEW_RANGE;
			
		}
	}

	
}

void worldDraw(World *this)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	
	glRotatef(this->player.rot[1]*180/M_PI,1,0,0);
	glRotatef(this->player.rot[0]*180/M_PI,0,0,1);

	glTranslatef(-this->player.pos[0],-this->player.pos[1],-this->player.pos[2]);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	
	glMatrixMode(GL_MODELVIEW);

	glBindTexture(GL_TEXTURE_2D, this->terrain);

	for(int z=0;z<VIEW_RANGE;z++)
	for(int y=0;y<VIEW_RANGE;y++)
	for(int x=0;x<VIEW_RANGE;x++)
	{
		if(this->segment[z][y][x]!=NULL)
		{
			glPushMatrix();
			glTranslatef((x+this->scroll[0])*SEGMENT_SIZE,(y+this->scroll[1])*SEGMENT_SIZE,(z+this->scroll[2])*SEGMENT_SIZE);
			drawSegment(this->segment[z][y][x]);
			glPopMatrix();
		}
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}
