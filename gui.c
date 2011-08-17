
#include "gui.h"
#include "utils.h"

static GLuint gui_texture=0;

void drawBackground(const Frame* frame, int background)
{

    Vec2f border={4,4};
    Vec2f texBorder={0.25,0.25};
    Vec2f texBase={background%16,background/16};

    Vec2f vertexCoord[4]=
    {
        {0,0},
        border,
        frame->size-border,
        frame->size,
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
        glTexCoord2f(texCoord[x  ][0],texCoord[y  ][1]);
        glVertex2f(vertexCoord[x  ][0],vertexCoord[y  ][1]);
        glTexCoord2f(texCoord[x+1][0],texCoord[y  ][1]);
        glVertex2f(vertexCoord[x+1][0],vertexCoord[y  ][1]);
        glTexCoord2f(texCoord[x+1][0],texCoord[y+1][1]);
        glVertex2f(vertexCoord[x+1][0],vertexCoord[y+1][1]);
        glTexCoord2f(texCoord[x  ][0],texCoord[y+1][1]);
        glVertex2f(vertexCoord[x  ][0],vertexCoord[y+1][1]);
    }

    glEnd();
}

void drawFrame(const Frame* frame)
{

    glPushMatrix();

    glTranslatef(frame->pos[0],frame->pos[1],0);



    switch(frame->type)
    {

        case CONTAINER:
            {

                Container* container=(Container*)frame;

                if(container->background)
                    drawBackground(frame,1);

                for(int i=0;i<container->childs;i++)
                    drawFrame(container->child[i]);

            }
            break;
        case SLOT:
            {
                drawBackground(frame,2);
            }
            break;
        case CANVAS:
            {
                drawBackground(frame,3);
            }
        default:
            break;
    }

    glPopMatrix();

}

public void windowDraw(Window* window)
{

    if(!window->visible)
        return;

    if(gui_texture==0)
    {
        gui_texture=loadTexture("gui.png");
        assert(gui_texture!=0);
    }

    glBindTexture(GL_TEXTURE_2D,gui_texture);
    glDisable(GL_CULL_FACE);

    drawFrame(window->frame);

}

public bool windowEvent(Window* window, SDL_Event* event)
{
    return false;
}


public Container equipment={
    {CONTAINER,{-88,-86},{176,166}},
    true,
    false,
    1+4+4+9*3+9+3,
    {
        &(Frame){SLOT,{144,36},{18,18}},

        &(Frame){SLOT,{88,26},{18,18}},
        &(Frame){SLOT,{88,26+18},{18,18}},
        &(Frame){SLOT,{88+18,26},{18,18}},
        &(Frame){SLOT,{88+18,26+18},{18,18}},

        &(Frame){SLOT,{7,7+18*0},{18,18}},
        &(Frame){SLOT,{7,7+18*1},{18,18}},
        &(Frame){SLOT,{7,7+18*2},{18,18}},
        &(Frame){SLOT,{7,7+18*3},{18,18}},

        &(Frame){SLOT,{7+18*0,83},{18,18}},
        &(Frame){SLOT,{7+18*1,83},{18,18}},
        &(Frame){SLOT,{7+18*2,83},{18,18}},
        &(Frame){SLOT,{7+18*3,83},{18,18}},
        &(Frame){SLOT,{7+18*4,83},{18,18}},
        &(Frame){SLOT,{7+18*5,83},{18,18}},
        &(Frame){SLOT,{7+18*6,83},{18,18}},
        &(Frame){SLOT,{7+18*7,83},{18,18}},
        &(Frame){SLOT,{7+18*8,83},{18,18}},

        &(Frame){SLOT,{7+18*0,83+18},{18,18}},
        &(Frame){SLOT,{7+18*1,83+18},{18,18}},
        &(Frame){SLOT,{7+18*2,83+18},{18,18}},
        &(Frame){SLOT,{7+18*3,83+18},{18,18}},
        &(Frame){SLOT,{7+18*4,83+18},{18,18}},
        &(Frame){SLOT,{7+18*5,83+18},{18,18}},
        &(Frame){SLOT,{7+18*6,83+18},{18,18}},
        &(Frame){SLOT,{7+18*7,83+18},{18,18}},
        &(Frame){SLOT,{7+18*8,83+18},{18,18}},

        &(Frame){SLOT,{7+18*0,83+18*2},{18,18}},
        &(Frame){SLOT,{7+18*1,83+18*2},{18,18}},
        &(Frame){SLOT,{7+18*2,83+18*2},{18,18}},
        &(Frame){SLOT,{7+18*3,83+18*2},{18,18}},
        &(Frame){SLOT,{7+18*4,83+18*2},{18,18}},
        &(Frame){SLOT,{7+18*5,83+18*2},{18,18}},
        &(Frame){SLOT,{7+18*6,83+18*2},{18,18}},
        &(Frame){SLOT,{7+18*7,83+18*2},{18,18}},
        &(Frame){SLOT,{7+18*8,83+18*2},{18,18}},

        &(Frame){SLOT,{7+18*0,142},{18,18}},
        &(Frame){SLOT,{7+18*1,142},{18,18}},
        &(Frame){SLOT,{7+18*2,142},{18,18}},
        &(Frame){SLOT,{7+18*3,142},{18,18}},
        &(Frame){SLOT,{7+18*4,142},{18,18}},
        &(Frame){SLOT,{7+18*5,142},{18,18}},
        &(Frame){SLOT,{7+18*6,142},{18,18}},
        &(Frame){SLOT,{7+18*7,142},{18,18}},
        &(Frame){SLOT,{7+18*8,142},{18,18}},

        &(Frame){CANVAS,{26,7},{54,72}},
        &(Frame){ICON,{125,37},{16,13}},
        &(Frame){LABEL,{86,16},{0,0}},
    }
};

Layouts layouts=
{
    .equipment=&equipment,
};
