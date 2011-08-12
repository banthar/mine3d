
#include "player.h"

#include "client.h"
#include "utils.h"
#include "model.h"

Box playerModel={
    {-4, -2, 12},
    {8, 4, 12},
    {16, 16},
    5,
    {
        {{4, 2, 12},&(Box){{-4,-4,0},{8,8,8},{0,0}}},

        {{-2, 2, 10},&(Box){{-2,-2,-10},{4,4,12},{40,16}}},
        {{10, 2, 10},&(Box){{-2,-2,-10},{4,4,12},{40,16}}},

        {{2, 2, 0},&(Box){{-2,-2,-12},{4,4,12},{0,16}}},
        {{6, 2, 0},&(Box){{-2,-2,-12},{4,4,12},{0,16}}},
    }
};

void humanDraw(Actor* actor)
{

    if(!actor->draw)
        return;

    static GLuint texture=0;
    if(texture==0)
        texture=loadTexture("data/mob/char.png");

    glBindTexture(GL_TEXTURE_2D,texture);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatev(actor->pos);
    glRotatef(actor->rot[0],0,0,1);
    glScalef(1.0/16.0,1.0/16.0,1.0/16.0);
    drawBox(&playerModel);
    glPopMatrix();
}

void playerTick(Client* client, Player* player)
{
}

public Player* playerNew()
{

    Player* player=calloc(sizeof(Player),1);

    *player=(Player){
        .actorType=777,
        .size={0.3,0.3,0.9},
        .headOffset={0.0,0.0,0.89},
        .draw=false,
    };

    return player;

}
