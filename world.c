
#include "config.h"

#include "world.h"

#include "utils.h"
#include "noise.h"
#include "block.h"

#include "SDL.h"
#include "glew.h"
#include <math.h>
#include <stdlib.h>
#include <pthread.h>

private Segment* newSegment();

public void worldLock(World* world)
{
	assert(world!=NULL);
	assert(SDL_LockMutex(world->lock)==0);
}

public void worldUnlock(World* world)
{
	assert(world!=NULL);
	assert(SDL_UnlockMutex(world->lock)==0);
}

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
			return (Block){};

	Segment* segment=this->segment[global[2]][global[1]][global[0]];

	if(segment==NULL)
		return (Block){};

	Vec4i local=pos&segment_mask;

	return segment->data[local[2]][local[1]][local[0]];

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

	segment->rendered=false;

	if(block.id!=0)
		segment->empty=false;

	Vec4i local=pos&segment_mask;

	segment->data[local[2]][local[1]][local[0]]=block;

}

private void renderSegment(World* world, Segment* this, Vec4i pos)
{

	assert(this!=NULL);

	this->rendered=true;

	if(this->empty)
		return;

	static Vertex data[SEGMENT_SIZE*SEGMENT_SIZE*SEGMENT_SIZE*6*4];
	const int max_vertices=sizeof(data)/sizeof(*data);

	int n=0;

	for(int z=0;z<SEGMENT_SIZE;z++)
	for(int y=0;y<SEGMENT_SIZE;y++)
	for(int x=0;x<SEGMENT_SIZE;x++)
	{

		Vec4i loc=pos*SEGMENT_SIZEV+(Vec4i){x,y,z};

		Block block=this->data[z][y][x];

		//if(worldGet(world,loc)!=0)
		if(block.id!=0)
		{

			static const Vec4i face[6][4]={
				{{0,0,0},{1,0,0},{1,1,0},{0,1,0}},
				{{0,0,1},{0,1,1},{1,1,1},{1,0,1}},

				{{1,0,0},{0,0,0},{0,0,1},{1,0,1}},
				{{0,1,0},{1,1,0},{1,1,1},{0,1,1}},

				{{0,0,0},{0,1,0},{0,1,1},{0,0,1}},
				{{1,1,0},{1,0,0},{1,0,1},{1,1,1}},
			};

			static const Vec4i tree_face[4][4]={
				{{1,0,0},{0,1,0},{0,1,1},{1,0,1}},
				{{0,1,0},{1,0,0},{1,0,1},{0,1,1}},
				{{1,1,0},{0,0,0},{0,0,1},{1,1,1}},
				{{0,0,0},{1,1,0},{1,1,1},{0,0,1}},
			};

			static const int texCoord[4][2]={
				{0,1},{1,1},{1,0},{0,0},
			};

			static const Vec4i normal[6]={
				{ 0, 0,-1},
				{ 0, 0, 1},
				{ 0,-1, 0},
				{ 0, 1, 0},
				{-1, 0, 0},
				{ 1, 0, 0},
			};

			switch(block_definition[block.id].draw_mode)
			{
				case DRAW_NONE:
					break;
				case DRAW_TREE:
					for(int i=0;i<4;i++)
					{
						int tile_id=block_definition[block.id].textures[i];

						int tileX=tile_id%TEXTURE_SIZE;
						int tileY=tile_id/TEXTURE_SIZE;

						for(int v=0;v<4;v++)
						{
							assert(n<max_vertices);
							data[n].pos=loc+tree_face[i][v];
							data[n].color=(Vec4f){1,1,1,1};
							if(block_definition[block.id].color_mode==COLOR_GRASS)
								data[n].color*=(Vec4f){0,1,0,1};
							data[n].texCoord=(Vec2f){(texCoord[v][0]+tileX)*1.0/TEXTURE_SIZE,(texCoord[v][1]+tileY)*1.0/TEXTURE_SIZE};
							n++;
						}

					}
					break;
				case DRAW_BLOCK:
					for(int i=0;i<6;i++)
					{

						Block normalBlock=worldGet(world,loc+normal[i]);			

						if(block_definition[normalBlock.id].transparent==false)
							continue;


						int tile_id=block_definition[block.id].textures[i];

						int tileX=tile_id%TEXTURE_SIZE;
						int tileY=tile_id/TEXTURE_SIZE;

						for(int v=0;v<4;v++)
						{
							assert(n<max_vertices);
							data[n].pos=loc+face[i][v];
							float light=clampf(normalBlock.light+normalBlock.skyLight,0.2,1);
							data[n].color=(Vec4f){light,light,light,1};
							if(block_definition[block.id].color_mode==COLOR_GRASS)
								data[n].color*=(Vec4f){0.2,1,0.1,1};

							data[n].texCoord=(Vec2f){(texCoord[v][0]+tileX)*1.0/TEXTURE_SIZE,(texCoord[v][1]+tileY)*1.0/TEXTURE_SIZE};
							n++;
						}

					}
					break;
				default:
					break;

			}

		}
	}

	if(n==0)
		return;

	this->n=n;

	if(this->vbo==0)
		glGenBuffers(1,&this->vbo);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*data)*n, data, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

private void drawSegment(World* world, Segment* this, Vec4i pos)
{

	if(this!=NULL && this->rendered && this->vbo!=0)
	{

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glVertexPointer(3, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex,pos));
		glColorPointer(4, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex,color));
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

private Segment* newSegment()
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

private void worldSpiral(World* this, void (f)(World*,int x,int y,int z))
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

public void worldTick(World* this)
{

	this->ticks=this->lastSyncTicks+(SDL_GetTicks()-this->lastSyncTime)*20/1000;

	//int t=SDL_GetTicks();	
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
		v=0.1;
	}
	
	this->player.pos[0]-=sin(this->player.rot[0])*sin(this->player.rot[1])*vy*v;
	this->player.pos[1]-=cos(this->player.rot[0])*sin(this->player.rot[1])*vy*v;
	//this->player.pos[2]-=cos(this->player.rot[1])*vy*v;

	this->player.pos[0]-=-cos(this->player.rot[0])*vx*v;
	this->player.pos[1]-=sin(this->player.rot[0])*vx*v;

	int id=worldGet(this,(Vec4i){this->player.pos[0],this->player.pos[1],this->player.pos[2]-1.6}).id;

	if(id==0)
	{
		this->player.pos[2]-=0.1;
		this->player.on_ground=false;
	}
	else
	{
		this->player.on_ground=true;
	}

/*
	while(worldGet(this,(Vec4i){this->player.pos[0],this->player.pos[1],this->player.pos[2]-3})!=0)
	{
		printf("%i\n",worldGet(this,(Vec4i){this->player.pos[0],this->player.pos[1],this->player.pos[2]}));
		this->player.pos[2]+=1.0;
	}
*/
	
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
	//	sendPlayerPositionAndLook(this, this->socket);

	//printf("ticks: %i ",SDL_GetTicks()-t);
	//printf("segments: %i ",allocated_segments);
	//printf("player: %f %f %f\n",this->player.pos[0],this->player.pos[1],this->player.pos[2]);

}

private void worldDrawSegment(World *this,int x, int y, int z)
{

	Vec4i pos=(Vec4i){x,y,z}+this->scroll;

/*
	if(this->segment[z][y][x]==NULL && SDL_GetTicks()-this->time<10)
	{
		this->segment[z][y][x]=newSegment();
		generateSegment(this, this->segment[z][y][x],(Vec4i){x,y,z}+this->scroll);
	}
*/

	if(	this->segment[z][y][x]!=NULL &&	this->segment[z][y][x]->rendered==false && SDL_GetTicks()-this->drawStart<10)
	{
		renderSegment(this, this->segment[z][y][x], pos);
	}

	drawSegment(this, this->segment[z][y][x], pos);

}


public void worldDraw(World *world)
{

	world->drawStart=SDL_GetTicks();

	char buf[1024];
	sprintf(buf,"x:%f y:%f z:%f (%f %f)",world->player.pos[0],world->player.pos[1],world->player.pos[2],world->player.rot[0],world->player.rot[1]);
	ftglRenderFont(world->font, buf, FTGL_RENDER_ALL);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	
	glRotatef(world->player.rot[1]*180/M_PI,1,0,0);
	glRotatef(world->player.rot[0]*180/M_PI,0,0,1);

	glTranslated(-world->player.pos[0],-world->player.pos[1],-world->player.pos[2]);

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

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_TEXTURE_2D);
	glAlphaFunc(GL_GREATER,0.1);

	//float daytime=(world->ticks%24000ull)/24000.0;

	glMatrixMode(GL_MODELVIEW);

	glBindTexture(GL_TEXTURE_2D, world->terrain);

	worldSpiral(world,worldDrawSegment);


	glMatrixMode(GL_PROJECTION);
	glPopMatrix();


}


public void worldInit(World *this)
{
	
	*this=(World){};

	this->lock=SDL_CreateMutex();
	this->writeLock=SDL_CreateMutex();

	noiseInit(&this->noise,666);

	this->terrain=loadTexture("terrain.png");
	assert(this->terrain!=0);

	this->font=ftglCreateBitmapFont("/usr/share/fonts/truetype/ttf-dejavu/DejaVuSansMono.ttf");
	assert(this->font!=NULL);
	ftglSetFontFaceSize(this->font, 18, 0);

	this->player.pos=(Vec4f){0,0,16};

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

