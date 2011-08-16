
#include "model.h"

#include "utils.h"

#include <stdlib.h>

public void drawBox(Box* box, Vec4f* rotations)
{

    static const Vec4f face_mask[6][4]={
        {{0,0,0},{1,0,0},{1,1,0},{0,1,0}},
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

    glPushMatrix();

    if(box->bone>=0 && rotations!=NULL)
    {
        glRotatef(rotations[box->bone][2],0,0,1);
        glRotatef(rotations[box->bone][1],0,1,0);
        glRotatef(rotations[box->bone][0],1,0,0);
    }

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
        drawBox(box->child[i].box,rotations);
        glPopMatrix();
    }

    glPopMatrix();

}
