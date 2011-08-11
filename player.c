
#include "player.h"

#include "client.h"
#include "utils.h"

typedef struct Box Box;

struct Box
{
    Vec4f offset;
    Vec4f size;
    Vec2f textureOffset;
    int childs;
    struct{
        Vec4f offset;
        Box* box;
    }child[];
};

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

void drawBox(Box* box)
{

    static const Vec4f face_mask[6][4]={
        {{1,0,0},{1,1,0},{0,1,0},{0,0,0}},
        {{0,1,1},{1,1,1},{1,0,1},{0,0,1}},

        {{1,0,0},{0,0,0},{0,0,1},{1,0,1}},
        {{0,1,0},{1,1,0},{1,1,1},{0,1,1}},

        {{0,0,0},{0,1,0},{0,1,1},{0,0,1}},
        {{1,1,0},{1,0,0},{1,0,1},{1,1,1}},
    };

    static const Vec2f texture_mask[4]={
        {0,1},{1,1},{1,0},{0,0},
    };

    Vec2f texture_offset[6]={
        {box->size[1]+box->size[0],0},
        {box->size[1],0},
        {box->size[0]+box->size[1]*2,box->size[1]},
        {box->size[1],box->size[1]},
        {0,box->size[1]},
        {box->size[0]+box->size[1],box->size[1]},
    };

    Vec2f texture_size[6]={
        {box->size[0],box->size[1]},
        {box->size[0],box->size[1]},
        {box->size[0],box->size[2]},
        {box->size[0],box->size[2]},
        {box->size[1],box->size[2]},
        {box->size[1],box->size[2]},
    };

    glBegin(GL_QUADS);
    for(int f=0;f<6;f++)
        for(int v=0;v<4;v++)
        {
            Vec2f tex=texture_offset[f]+box->textureOffset+texture_mask[v]*texture_size[f];
            glTexCoord2f(tex[0]/64.0,tex[1]/32.0);
            glVertexf(box->offset+box->size*face_mask[f][v]);
        }
    glEnd();

    for(int i=0;i<box->childs;i++)
    {
        glPushMatrix();
        glTranslatev(box->child[i].offset+box->offset);
        glRotatef((rand()/(float)RAND_MAX-0.5)*10,0,0,1);
        glRotatef((rand()/(float)RAND_MAX-0.5)*10,0,1,0);
        glRotatef((rand()/(float)RAND_MAX-0.5)*10,1,0,0);
        drawBox(box->child[i].box);
        glPopMatrix();
    }

}

void modelDraw(Client* client)
{
    glBindTexture(GL_TEXTURE_2D,client->zombieTexture);
    glScalef(1.0/16.0,1.0/16.0,1.0/16.0);

    drawBox(&playerModel);

}

void playerTick(Client* client, Player* player)
{
}

