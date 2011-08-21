
#pragma once

#include <math.h>

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)>(b)?(b):(a))

#define max3(a,b,c) max(a,max(b,c))
#define min3(a,b,c) min(a,min(b,c))

static inline double clampf(double v, double min, double max)
{
    if(v<min)
        return min;
    else if(v>max)
        return max;
    else
        return v;
}

static inline double clamp(int v, int min, int max)
{
    if(v<min)
        return min;
    else if(v>max)
        return max;
    else
        return v;
}

static inline float fract(float x)
{
    return x>0?x-floor(x):x-floor(x);
}

static inline float sign(float x)
{
    return x>0?1:x==0?0:-1;
}

#define VECTOR(type,size) type __attribute__ ((vector_size ((size)*sizeof(type))))

typedef VECTOR(int,2) Vec2i;
#define vec2i(args...) (Vec2i){args}
typedef VECTOR(int,4) Vec4i;
#define vec4i(args...) (Vec4i){args}

typedef VECTOR(float,2) Vec2f;
#define vec2f(args...) (Vec2f){args}
typedef VECTOR(float,4) Vec4f;
#define vec4f(args...) (Vec4f){args}

typedef VECTOR(unsigned char,2) Vec2b;
#define vec2b(args...) (Vec2b){args}
typedef VECTOR(unsigned char,4) Vec4b;
#define vec4b(args...) (Vec4b){args}

typedef VECTOR(short,2) Vec2s;
#define vec2s(args...) (Vec2s){args}
typedef VECTOR(short,4) Vec4s;
#define vec4s(args...) (Vec4s){args}

typedef struct
{
    Vec4f data[4];
}Matrix;

extern const Matrix matrixIdentity;
extern const Matrix matrixZero;

Vec4f matrixProject(const Matrix* m, const Vec4f v);
Matrix matrixMultiply(const Matrix *this, const Matrix *that);
Matrix matrixScale(const Matrix *m, Vec4f v);
Matrix matrixTranslate(const Matrix *m, Vec4f v);
Matrix matrixRotateX(const Matrix *m, const float a);
Matrix matrixRotateY(const Matrix *m, const float a);
Matrix matrixRotateZ(const Matrix *m, const float a);
Matrix matrixFrustum(const Matrix *m, const float near, const float far);
void matrixPrint(const Matrix *m);

static inline float length4f(Vec4f v)
{
    return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]+v[3]*v[3]);
}


static inline Vec4f rotationNormal(Vec2f rot)
{

    return (Vec4f){
        -sin(rot[0]/180.0*M_PI)*sin(rot[1]/180.0*M_PI),
        cos(rot[0]/180.0*M_PI)*sin(rot[1]/180.0*M_PI),
        -cos(rot[1]/180.0*M_PI),
    };
}


