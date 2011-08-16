
#include "gui.h"
#include "utils.h"

static GLuint gui_texture=0;

typedef struct Frame Frame;
typedef struct Rectangle Rectangle;

struct Rectangle
{
    Vec2f pos;
    Vec2f size;
};

struct Frame
{
    Vec2f pos;
    Vec2f size;
    int type;
};


void drawFrame(Frame* frame)
{

    glBindTexture(GL_TEXTURE_2D,gui_texture);
    //glMatrixMode(GL_MODELVIEW);
    //glPushMatrix();

    //glLoadIdentity();
    glDisable(GL_CULL_FACE);

    //glTranslatef(frame->pos[0],frame->pos[1],0);

    Vec2f border={0.033,0.033};

    Vec2f texBorder={0.25,0.25};
    Vec2f texBase={frame->type%16,frame->type/16};

    Vec2f vertexCoord[4]=
    {
        frame->pos,
        frame->pos+border,
        frame->pos+frame->size-border,
        frame->pos+frame->size,
    };

    Vec2f texCoord[4]=
    {
        (texBase)/(Vec2f){16,16},
        (texBase+texBorder)/(Vec2f){16,16},
        (texBase+(Vec2f){1,1}-texBorder)/(Vec2f){16,16},
        (texBase+(Vec2f){1,1})/(Vec2f){16,16},
    };

    glBegin(GL_QUADS);

    for(int y=0;y<3;y++)
    for(int x=0;x<3;x++)
    {
            glTexCoord2f(texCoord[x  ][0],texCoord[3-y  ][1]);
            glVertex2f(vertexCoord[x  ][0],vertexCoord[y  ][1]);
            glTexCoord2f(texCoord[x+1][0],texCoord[3-y  ][1]);
            glVertex2f(vertexCoord[x+1][0],vertexCoord[y  ][1]);
            glTexCoord2f(texCoord[x+1][0],texCoord[3-y-1][1]);
            glVertex2f(vertexCoord[x+1][0],vertexCoord[y+1][1]);
            glTexCoord2f(texCoord[x  ][0],texCoord[3-y-1][1]);
            glVertex2f(vertexCoord[x  ][0],vertexCoord[y+1][1]);
    }

    glEnd();

    /*
    glBegin(GL_QUADS);

        glTexCoordf(texCoord+(Vec2f){0,0});
        glVertex2f(0,0);
        glTexCoordf(texCoord+(Vec2f){1/16.0,0});
        glVertex2f(0,frame->size[1]);
        glTexCoordf(texCoord+(Vec2f){1/16.0,1/16.0});
        glVertex2f(frame->size[0],frame->size[1]);
        glTexCoordf(texCoord+(Vec2f){0,1/16.0});
        glVertex2f(frame->size[0],0);

    glEnd();
    */

    //glPopMatrix();

}

void guiDraw()
{

    Frame frame={
        .pos={-0.75,-0.75},
        .size={1.5,1.5},
        .type=0,
    };

    if(gui_texture==0)
    {
        gui_texture=loadTexture("gui.png");
        assert(gui_texture!=0);
    }

    drawFrame(&frame);

}
