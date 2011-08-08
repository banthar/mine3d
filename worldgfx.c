
#include "world.h"

#include "utils.h"

private void renderSegment(World* world, Segment* this, Vec4i pos)
{

    assert(this!=NULL);

    this->rendered=true;

    if(this->empty)
        return;

    static Vertex data[SEGMENT_SIZE*SEGMENT_SIZE*SEGMENT_SIZE*6*4];

    VertexBuffer buffer=
    {
        .maxSize=sizeof(data)/sizeof(*data),
        .size=0,
        .data=data,
    };

    for(int z=0;z<SEGMENT_SIZE;z++)
    for(int y=0;y<SEGMENT_SIZE;y++)
    for(int x=0;x<SEGMENT_SIZE;x++)
    {
        blockDraw(world,pos*SEGMENT_SIZEV+(Vec4i){x,y,z},&buffer);
    }

    if(buffer.size==0)
        return;

    this->n=buffer.size;

    if(this->vbo==0)
        glGenBuffers(1,&this->vbo);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*data)*this->n, data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

private void drawSegment(World* world, Segment* this, Vec4i pos)
{

    if(this!=NULL && this->vbo!=0)
    {

        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        glVertexPointer(3, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex,pos));
        glColorPointer(4, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex,color));
        glTexCoordPointer(2,GL_FLOAT,sizeof(Vertex), (void*)offsetof(Vertex,texCoord));

        glDrawArrays(GL_QUADS, 0, this->n);

    }

}


private void drawSelection(World* world, Vec4i pos)
{
    static const Vec4i face[6][4]={
        {{0,0,0},{0,1,0},{0,1,1},{0,0,1}},
        {{1,1,0},{1,0,0},{1,0,1},{1,1,1}},

        {{1,0,0},{0,0,0},{0,0,1},{1,0,1}},
        {{0,1,0},{1,1,0},{1,1,1},{0,1,1}},

        {{0,0,0},{1,0,0},{1,1,0},{0,1,0}},
        {{0,0,1},{0,1,1},{1,1,1},{1,0,1}},
    };

    glDisable(GL_DEPTH_TEST);
    glBegin(GL_LINE_STRIP);
    for(int i=0;i<4;i++)
        glVertexi(pos+face[pos[3]][i]);
    glVertexi(pos+face[pos[3]][0]);
    glEnd();

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

    if( this->segment[z][y][x]!=NULL && this->segment[z][y][x]->rendered==false && SDL_GetTicks()-this->drawStart<10)
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
    //ftglRenderFont(world->font, buf, FTGL_RENDER_ALL);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    //glTranslated(0,0,-3);


    glRotatef(world->player.rot[1]*180/M_PI,1,0,0);
    glRotatef(world->player.rot[0]*180/M_PI,0,0,1);

    glTranslatev(-world->player.pos-world->player.headOffset);

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

    worldTick(world);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_TEXTURE_2D);
    glAlphaFunc(GL_GREATER,0.1);

    //float daytime=(world->ticks%24000ull)/24000.0;

    glMatrixMode(GL_MODELVIEW);

    glBindTexture(GL_TEXTURE_2D, world->terrain);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    worldSpiral(world,worldDrawSegment);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);


    Vec4i p=worldRay(world, world->player.pos+world->player.headOffset,  rotationNormal(world->player.rot),3);

    if(p[3]!=-1)
        drawSelection(world,p);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

}

public void worldDisplayInit(World *this)
{
    this->terrain=loadTexture("data/terrain.png");
    assert(this->terrain!=0);

    this->font=ftglCreateBitmapFont("/usr/share/fonts/truetype/ttf-dejavu/DejaVuSansMono.ttf");
    assert(this->font!=NULL);
    ftglSetFontFaceSize(this->font, 18, 0);
}
