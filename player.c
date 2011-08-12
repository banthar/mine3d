
#include "player.h"

#include "client.h"
#include "utils.h"
#include "model.h"

Box playerModel={
    {-4, -2, -6},
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


void modelDraw(Client* client)
{
    glBindTexture(GL_TEXTURE_2D,client->zombieTexture);
    glScalef(1.0/16.0,1.0/16.0,1.0/16.0);

    drawBox(&playerModel);

}

void playerTick(Client* client, Player* player)
{
}

public Player* playerNew()
{
    return calloc(sizeof(Player),1);
}
