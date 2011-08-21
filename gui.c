
#include "gui.h"
#include "utils.h"
#include "client.h"

static GLuint gui_texture=0;

void drawBackground(Vec2f pos, Vec2f size, int background)
{

    Vec2f border={4,4};
    Vec2f texBorder={0.25,0.25};
    Vec2f texBase={background%16,background/16};

    Vec2f vertexCoord[4]=
    {
        pos,
        pos+border,
        pos+size-border,
        pos+size,
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

void drawSlot(const Slot* slot)
{
    drawBackground(slot->pos,slot->size,2);
}

void drawLabel(const Label* label)
{

}

void drawIcon(const Icon* icon)
{

}

public bool windowEvent(Window* window, SDL_Event* event)
{
    return false;
}

/*
public bool equipmentEvent(Client* client, SDL_Event* event)
{
    return true;
}

public void equipmentDraw(Client* client)
{

    glPushMatrix();

    glTranslatef(equipmentFrame.pos[0],equipmentFrame.pos[1],0);
    drawBackground((Vec2f){0,0},equipmentFrame.size,1);

    for(int i=0;i<lengthof(equipmentSlots);i++)
        drawSlot(&equipmentSlots[i]);

    drawLabel(&equipmentCraftingLabel);
    drawIcon(&equipmentCraftingIcon);

    drawBackground(equipmentPlayerView.pos,equipmentPlayerView.size,3);

    glPopMatrix();

}

private float guiScale(Client* client)
{

    float scale = min(client->screen->w,client->screen->h)/240.0;

    if(scale>=1)
        scale=floor(scale);
    else
        scale=pow(2,floor(log2(scale)));

    return scale;

}

public void guiDraw(Client* client)
{

    if(gui_texture==0)
    {
        gui_texture=loadTexture("gui.png");
        assert(gui_texture!=0);
    }


    glBindTexture(GL_TEXTURE_2D,gui_texture);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    float scale=guiScale(client);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-client->screen->w/2/scale, client->screen->w/2/scale, client->screen->h/2/scale, -client->screen->h/2/scale, -1, 1);

    equipmentDraw(client);

    glPopMatrix();

}

public void guiEvent(Client* client,SDL_Event* event)
{

    switch(event->type)
    {
        case SDL_MOUSEMOTION:
            if(!client->grab_mouse)
            {
                float scale=guiScale(client);
                Vec2f mouse={event->motion.x/(float)client->screen->w,event->motion.y/(float)client->screen->h};
                mouse-=(Vec2f){0.5,0.5};
                mouse*=(Vec2f){scale,scale};

                printf("%f %f\n",mouse[0],mouse[1]);
            }
            break;

    }

}
*/
