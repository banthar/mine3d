
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
*/

/*

*/

private void componentDraw(Window* window, Component* component)
{

    switch(component->type)
    {
        case SLOT:
            drawBackground(component->pos,component->size,2);
            break;
        case BUTTON:
            {
                int background;

                if(!((Button*)component)->enabled)
                    background=8;
                else if(window->focus==component)
                    background=6;
                else
                    background=7;

                drawBackground(component->pos,component->size,background);

            }
            break;
        default:
            panic("unable to draw component type='%i'",component->type);
    }

}

private void containerDraw(Window* window, Container* container)
{
    int child=container->firstChild;

    while(child>=0)
    {
        componentDraw(window,&window->component[child]);
        child=window->component[child].nextChild;
    }
}

public void windowDraw(Window* window)
{

    if(gui_texture==0)
    {
        gui_texture=loadTexture("gui.png");
        assert(gui_texture!=0);
    }


    glBindTexture(GL_TEXTURE_2D,gui_texture);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glPushMatrix();
    glTranslatef(window->pos[0],window->pos[1],0);

    if(window->background)
        drawBackground((Vec2f){0,0},window->size,1);

    containerDraw(window,window);

    glPopMatrix();

}

private bool componentContains(Component* component, Vec2f point)
{

    return
        point[0] >= component->pos[0] &&
        point[1] >= component->pos[1] &&
        point[0] <  component->pos[0]+component->size[0] &&
        point[1] <  component->pos[1]+component->size[1];

}

private bool buttonEvent(Window* window, int id, Event* event)
{

    Button* button=&window->component[id].button;

    window->focus=button;

    if(event->type==MOUSE_DOWN && button->enabled)
        printf("%i\n",id);

    return false;

}

private bool componentEvent(Window* window, int id, Event* event)
{

    switch(window->component[id].type)
    {
        case BUTTON:
            return buttonEvent(window,id,event);
        default:
            return true;
    }

    return true;

}

private bool containerEvent(Window* window, Container* container, Event* event)
{

    int child=container->firstChild;

    while(child>=0)
    {

        Component* component=&window->component[child];

        if(componentContains(component,event->mouse))
        {
            Event tmpEvent=*event;
            tmpEvent.mouse-=container->pos;
            componentEvent(window,child,&tmpEvent);
        }

        child=component->nextChild;
    }

    return false;
}

public bool windowEvent(Window* window, Event* event)
{

    //printf("event: %i\n",event->type);

    if(event->type==MOUSE_MOTION)
        window->focus=NULL;

    if(event->type==MOUSE_MOTION || event->type==MOUSE_UP || event->type==MOUSE_DOWN)
    {
        Event tmpEvent=*event;
        tmpEvent.mouse-=window->pos;
        return containerEvent(window,window,&tmpEvent);
    }
    else
    {
        return true;
    }
}

