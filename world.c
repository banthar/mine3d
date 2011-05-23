
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
Segment* loadSegment(int x, int y, int z);

static __attribute__ ((pure)) unsigned int noise3D(Vec4i v) 
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

void vboSegment(World* world, Segment* this, Vec4i pos);

void generateSegment(World* world, Segment* segment, Vec4i pos)
{

	for(int y=0;y<SEGMENT_SIZE;y++)
	for(int x=0;x<SEGMENT_SIZE;x++)
	{
		
		Vec4f xy=(Vec4f){x+pos[0]*SEGMENT_SIZE,y+pos[1]*SEGMENT_SIZE};

		float height=noise3(world->noise,xy*(Vec4f){0.01,0.01,0.01,0.01})*50;
		height+=noise3(world->noise,xy*(Vec4f){0.1,0.1,0.1,0.1})*5;
		height+=noise3(world->noise,xy*(Vec4f){0.001,0.001,0.001,0.001})*100;

		for(int z=0;z<SEGMENT_SIZE;z++)
		{

			Vec4f xyz=(Vec4f){x+pos[0]*SEGMENT_SIZE,y+pos[1]*SEGMENT_SIZE,z+pos[2]*SEGMENT_SIZE};

			if(height>xyz[2])
			{
				if(noise3(world->noise,xyz*(Vec4f){0.01,0.01,0.01,0.01})>0.1)
					segment->data[z][y][x]=2;
			}
			else
			{
				segment->data[z][y][x]=0;
			}
	
		}
	}

}

typedef struct
{
	Vec4i pos;
	Vec4b color;
	Vec2f texCoord;
}Vertex;

int segmentGet(Segment* this, int x, int y, int z)
{
	if(x<0 || y<0 || z<0 || x>=SEGMENT_SIZE || y>=SEGMENT_SIZE || z>=SEGMENT_SIZE)
		return -1;
	else
		return this->data[z][y][x];
}

int worldGet(World* this, Vec4i pos)
{

	const Vec4i segment_bits=(Vec4i){SEGMENT_BITS,SEGMENT_BITS,SEGMENT_BITS,SEGMENT_BITS};
	const Vec4i segment_mask=(Vec4i){SEGMENT_MASK,SEGMENT_MASK,SEGMENT_MASK,SEGMENT_MASK};

	Vec4i global=(pos>>segment_bits)-this->scroll;

	for(int i=0;i<3;i++)
		if(global[i]<0 || global[i]>=VIEW_RANGE)
			return 0;

	Segment* segment=this->segment[global[2]][global[1]][global[0]];

	if(segment==NULL)
		return 0;

	Vec4i local=pos&segment_mask;

	return segment->data[local[2]][local[1]][local[0]];

}

void renderSegment(World* world, Segment* this, Vec4i pos)
{

	assert(this!=NULL);

	const int max_vertices=SEGMENT_SIZE*SEGMENT_SIZE*SEGMENT_SIZE*6*4;

	Vertex data[max_vertices];

	int n=0;

	for(int z=0;z<SEGMENT_SIZE;z++)
	for(int y=0;y<SEGMENT_SIZE;y++)
	for(int x=0;x<SEGMENT_SIZE;x++)
	{

		Vec4i loc=pos*SEGMENT_SIZEV+(Vec4i){x,y,z};

		//if(worldGet(world,loc)!=0)
		if(this->data[z][y][x]!=0)
		{

			static const Vec4i face[6][4]={
				{{0,0,0},{1,0,0},{1,1,0},{0,1,0}},
				{{0,0,1},{0,1,1},{1,1,1},{1,0,1}},

				{{1,0,0},{0,0,0},{0,0,1},{1,0,1}},
				{{0,1,0},{1,1,0},{1,1,1},{0,1,1}},

				{{0,0,0},{0,1,0},{0,1,1},{0,0,1}},
				{{1,1,0},{1,0,0},{1,0,1},{1,1,1}},
			};

			static const int texCoord[4][2]={
				{0,0},{1,0},{1,1},{0,1},
			};

			static const Vec4i normal[6]={
				{0,0,-1},
				{0,0,1},
				{0,-1,0},
				{0,1,0},
				{-1,0,0},
				{1,0,0},
			};

			static const Vec4b colors[]={
				{96,96,96},
				{255,255,255},
				{128,128,128},
				{160,160,160},
				{192,192,192},
				{224,224,224},
			};

			int tileX=(this->data[z][y][x]-1)%TEXTURE_SIZE;
			int tileY=(this->data[z][y][x]-1)/TEXTURE_SIZE;

			for(int i=0;i<6;i++)
			{
			
				if(worldGet(world,loc+normal[i]) != 0)
					continue;
				
				for(int v=0;v<4;v++)
				{
					assert(n<max_vertices);
					data[n].pos=loc+face[i][v];
					data[n].color=colors[i];
					data[n].texCoord=(Vec2f){(texCoord[v][0]+tileX)*1.0/TEXTURE_SIZE,(texCoord[v][1]+tileY)*1.0/TEXTURE_SIZE};
					//data[n].texCoord=(Vec2s){texCoord[v][0],texCoord[v][1]};
					n++;
				}
			}

		}
	}

	this->rendered=true;

	if(n==0)
		return;

	this->n=n;
	glGenBuffers(1,&this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*data)*n, data, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}

void drawSegment(World* world, Segment* this, Vec4i pos)
{

	if(this!=NULL && this->rendered && this->vbo!=0)
	{

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glVertexPointer(3, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex,pos));
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex,color));
		glTexCoordPointer(2,GL_FLOAT,sizeof(Vertex), (void*)offsetof(Vertex,texCoord));

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glDrawArrays(GL_QUADS, 0, this->n);

	}
	else if( false && (this==NULL || this->rendered==false))
	{

		static const Vec4i faces[6][4]={
			{{0,0,0},{1,0,0},{1,1,0},{0,1,0}},
			{{0,0,1},{0,1,1},{1,1,1},{1,0,1}},

			{{0,0,0},{0,0,1},{1,0,1},{1,0,0}},
			{{0,1,0},{1,1,0},{1,1,1},{0,1,1}},

			{{0,0,0},{0,1,0},{0,1,1},{0,0,1}},
			{{1,0,0},{1,0,1},{1,1,1},{1,1,0}},
		};

		glBegin(GL_LINES);
		for(int face=0;face<6;face++)
		for(int vert=0;vert<4;vert++)
		{
			Vec4i v=(pos+faces[face][vert])*SEGMENT_SIZEV;
			if(this==NULL)
				glColor3f(1.0,0.0,0.0);
			else if(this->rendered==false)
				glColor3f(1.0,1.0,0.0);
			else
				glColor3f(0.0,1.0,0.0);
				
			glVertex3i(v[0],v[1],v[2]);
		}

		glEnd();

	}

}

static int allocated_segments=0;

Segment* newSegment()
{
	allocated_segments++;
	return calloc(1,sizeof(Segment));
}

void freeSegment(Segment* this)
{
	
	if(this==NULL)
		return;
	
	allocated_segments--;

	if(this->vbo!=0)
		glDeleteBuffers(1,&this->vbo);
		 
	free(this);
	
}

void worldInit(World *this)
{
	
	*this=(World){};
	
	this->noise=newNoise(time(NULL));

	this->terrain=loadTexture("terrain.png");
	assert(this->terrain!=0);


}

void worldDestroy(World* this)
{
	free(this->noise);

	glDeleteTextures(1,&this->terrain);

	for(int z=0;z<VIEW_RANGE;z++)
	for(int y=0;y<VIEW_RANGE;y++)
	for(int x=0;x<VIEW_RANGE;x++)
	{
		freeSegment(this->segment[z][y][x]);
	}

}

static inline void worldSpiral(World* this, void (f)(int x,int y,int z))
{
	
	int c=VIEW_RANGE/2;
	
	for(int r=0;r<c;r++)
	for(int y=-r;y<r;y++)
	for(int x=-r;x<r;x++)
	{
		if(y==-r || x==-r || y==r-1 || x==r-1)
		{
			for(int z=-r;z<r;z++)
				f(c+x,c+y,c+z);
		}
		else
		{
			f(c+x,c+y,c+r-1);
			f(c+x,c+y,c-r);
		}
	}
	
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

	int t=SDL_GetTicks();	
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
	if(keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT])
	{
		v=0.01*this->player.pos[0];
	}
	
	this->player.pos[0]-=sin(this->player.rot[0])*sin(this->player.rot[1])*vy*v;
	this->player.pos[1]-=cos(this->player.rot[0])*sin(this->player.rot[1])*vy*v;
	this->player.pos[2]-=cos(this->player.rot[1])*vy*v;

	this->player.pos[0]-=-cos(this->player.rot[0])*vx*v;
	this->player.pos[1]-=sin(this->player.rot[0])*vx*v;

	while(worldGet(this,(Vec4i){this->player.pos[0],this->player.pos[1],this->player.pos[2]-3})!=0)
	{
		printf("%i\n",worldGet(this,(Vec4i){this->player.pos[0],this->player.pos[1],this->player.pos[2]}));
		this->player.pos[2]+=1.0;
	}
	
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
/*
	//for(int r=0;r<VIEW_RANGE/2;r++)
	for(int z=0;z<VIEW_RANGE;z++)
	for(int y=0;y<VIEW_RANGE;y++)
	for(int x=0;x<VIEW_RANGE;x++)
	{

		//if(max3(abs(x-VIEW_RANGE/2),abs(y-VIEW_RANGE/2),abs(z-VIEW_RANGE/2))!=r)
			//continue;

		if(this->segment[z][y][x]==NULL)
		{
			this->segment[z][y][x]=newSegment();
			generateSegment(this, this->segment[z][y][x],(Vec4i){x,y,z}+this->scroll);
			
			if(SDL_GetTicks()-t>=5)
				x=y=z=VIEW_RANGE;
			
		}
	}
*/

	printf("ticks: %i ",SDL_GetTicks()-t);
	printf("segments: %i ",allocated_segments);
	printf("player: %f %f %f\n",this->player.pos[0],this->player.pos[1],this->player.pos[2]);

}

void worldDraw(World *this)
{

	int t=SDL_GetTicks();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	
	glRotatef(this->player.rot[1]*180/M_PI,1,0,0);
	glRotatef(this->player.rot[0]*180/M_PI,0,0,1);

	glTranslated(-this->player.pos[0],-this->player.pos[1],-this->player.pos[2]);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	
	glMatrixMode(GL_MODELVIEW);

	glBindTexture(GL_TEXTURE_2D, this->terrain);

	void aux(int x, int y, int z)
	{

		/*
		if(SDL_GetTicks()-t>25)
		{
			x=y=z=r=VIEW_RANGE;
			return false;
		}*/

		Vec4i pos=(Vec4i){x,y,z}+this->scroll;

		if(this->segment[z][y][x]==NULL && SDL_GetTicks()-t<10)
		{
			this->segment[z][y][x]=newSegment();
			generateSegment(this, this->segment[z][y][x],(Vec4i){x,y,z}+this->scroll);
		}

		if(
			this->segment[z][y][x]!=NULL &&
			this->segment[z][y][x]->rendered==false &&
			SDL_GetTicks()-t<20 &&

			this->segment[z+1][y][x]!=NULL &&
			this->segment[z-1][y][x]!=NULL &&

			this->segment[z][y+1][x]!=NULL &&
			this->segment[z][y-1][x]!=NULL &&

			this->segment[z][y][x+1]!=NULL &&
			this->segment[z][y][x-1]!=NULL )
		{
			renderSegment(this, this->segment[z][y][x], pos);
		}

		drawSegment(this, this->segment[z][y][x], pos);

/*
		if(this->segment[z][y][x]!=NULL)
		{
			//glPushMatrix();
			//glTranslatef((x+this->scroll[0])*SEGMENT_SIZE,(y+this->scroll[1])*SEGMENT_SIZE,(z+this->scroll[2])*SEGMENT_SIZE);
			drawSegment(this, this->segment[z][y][x], (Vec4i){x,y,z}+this->scroll);
			//glPopMatrix();
		}
*/

	}

	worldSpiral(this,aux);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

