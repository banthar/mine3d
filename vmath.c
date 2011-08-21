
#include "vmath.h"

#include <stdio.h>
#include <math.h>

const Matrix matrixIdentity={{
    {1,0,0,0},
    {0,1,0,0},
    {0,0,1,0},
    {0,0,0,1}
}};

const Matrix matrixZero={{
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
}};

Vec4f matrixProject(const Matrix* m, const Vec4f v)
{
    return vec4f(
            v[0]*m->data[0][0]+v[1]*m->data[0][1]+v[2]*m->data[0][2]+v[3]*m->data[0][3],
            v[0]*m->data[1][0]+v[1]*m->data[1][1]+v[2]*m->data[1][2]+v[3]*m->data[1][3],
            v[0]*m->data[2][0]+v[1]*m->data[2][1]+v[2]*m->data[2][2]+v[3]*m->data[2][3],
            v[0]*m->data[3][0]+v[1]*m->data[3][1]+v[2]*m->data[3][2]+v[3]*m->data[3][3]
        );
}

Matrix matrixMultiply(const Matrix *this, const Matrix *that)
{

    Matrix result=matrixZero;

    for(int y=0; y<4; y++)
    for(int x=0; x<4; x++)
    {
        for(int i=0; i<4; i++)
        {
            result.data[y][x]+=this->data[y][i]*that->data[i][x];
        }
    }

    return result;
}

Matrix matrixScale(const Matrix *m, Vec4f v)
{
    const Matrix scaleMatrix={{
            {v[0],    0,    0,   0},
            {   0, v[1],    0,   0},
            {   0,    0, v[2],   0},
            {   0,    0,    0,   1}
        }};

    return matrixMultiply(m,&scaleMatrix);

}

Matrix matrixTranslate(const Matrix *m, Vec4f v)
{
    const Matrix translationMatrix={{
            {1, 0, 0, v[0]},
            {0, 1, 0, v[1]},
            {0, 0, 1, v[2]},
            {0, 0, 0,    1}
        }};

    return matrixMultiply(m,&translationMatrix);

}

Matrix matrixRotateX(const Matrix *m, const float a)
{
    const Matrix rotationMatrix={{
            {1,      0,       0,   0},
            {0, cosf(a), -sinf(a), 0},
            {0, sinf(a),  cosf(a), 0},
            {0,      0,       0,   1}
        }};

    return matrixMultiply(m,&rotationMatrix);
}

Matrix matrixRotateY(const Matrix *m, const float a)
{
    const Matrix rotationMatrix={{
            { cosf(a), 0, sinf(a), 0},
            {       0, 1,       0, 0},
            {-sinf(a), 0, cosf(a), 0},
            {       0, 0,       0, 1}
        }};

    return matrixMultiply(m,&rotationMatrix);
}

Matrix matrixRotateZ(const Matrix *m, const float a)
{
    const Matrix rotationMatrix={{
            {cosf(a), -sinf(a), 0, 0},
            {sinf(a),  cosf(a), 0, 0},
            {      0,        0, 1, 0},
            {      0,        0, 0, 1}
        }};

    return matrixMultiply(m,&rotationMatrix);
}

Matrix matrixFrustum(const Matrix *m, const float near, const float far)
{

    const Matrix frustumMatrix={{
            {1, 0,                             0, 0},
            {0, 1,                             0, 0},
            {0, 0,         (far+near)/(far-near),-1},
            {0, 0, (2 * far * near)/(far - near), 0}
        }};

    return matrixMultiply(m,&frustumMatrix);
}

void matrixPrint(const Matrix *m)
{
    for(int y=0;y<4;y++)
    {
        for(int x=0;x<4;x++)
        {
            printf("%f ",m->data[y][x]);
        }

        printf("\n");

    }

}

