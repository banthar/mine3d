
#include "config.h"

#include "noise.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>


static const Vec2f grad2[] = {
	{ 1, 1},{-1, 1},{ 1,-1},{-1,-1},
	{ 1, 0},{-1, 0},{ 1, 0},{-1, 0},
	{ 0, 1},{ 0,-1},{ 0, 1},{ 0,-1}
};

static const Vec4f grad3[] = {
	{ 1, 1, 0},{-1, 1, 0},{ 1,-1, 0},{-1,-1, 0},
	{ 1, 0, 1},{-1, 0, 1},{ 1, 0,-1},{-1, 0,-1},
	{ 0, 1, 1},{ 0,-1, 1},{ 0, 1,-1},{ 0,-1,-1}
};

static const Vec4f grad4[]= {
	{ 0, 1, 1, 1}, { 0, 1, 1,-1}, { 0, 1,-1, 1}, { 0, 1,-1,-1},
	{ 0,-1, 1, 1}, { 0,-1, 1,-1}, { 0,-1,-1, 1}, { 0,-1,-1,-1},
	{ 1, 0, 1, 1}, { 1, 0, 1,-1}, { 1, 0,-1, 1}, { 1, 0,-1,-1},
	{-1, 0, 1, 1}, {-1, 0, 1,-1}, {-1, 0,-1, 1}, {-1, 0,-1,-1},
	{ 1, 1, 0, 1}, { 1, 1, 0,-1}, { 1,-1, 0, 1}, { 1,-1, 0,-1},
	{-1, 1, 0, 1}, {-1, 1, 0,-1}, {-1,-1, 0, 1}, {-1,-1, 0,-1},
	{ 1, 1, 1, 0}, { 1, 1,-1, 0}, { 1,-1, 1, 0}, { 1,-1,-1, 0},
	{-1, 1, 1, 0}, {-1, 1,-1, 0}, {-1,-1, 1, 0}, {-1,-1,-1, 0}
};

public void noiseInit(Noise* noise, unsigned int seed)
{
	for(int i = 0; i < 256; ++i)
	{
		noise->perm[i] = rand_r(&seed);
	}
}

private float dot(Vec2f a, Vec2f b)
{
	return a[0]*b[0]+a[1]*b[1];
}


private int hash(Vec4i seed)
{

	return (((seed[0]&1)+(seed[1]&1)))*0x3fffffff;

	uint8_t* p=(uint8_t*)&seed;

	unsigned int seedp=0xf2031201;

	for(int i=0;i<sizeof(Vec4i);i++)
	{
		int r=rand_r(&seedp);
		seedp^=r^p[i];
	}

	return rand_r(&seedp);

}


private int fastFloor(float x)
{
	return x>0.0?(int)x:(int)x-1;
}


public float noise2(const Noise* noise, Vec2f pos)
{

	const float F2=(sqrt(3.0)-1.0)/2.0;
	const float G2=(3.0-sqrt(3.0))/6.0;

	Vec2i skewed[3];
	Vec2f unskewed[3];
	Vec2f offset[3];

	float s=(pos[0]+pos[1])*F2;
	skewed[0]=(Vec2i){fastFloor(pos[0]+s),fastFloor(pos[1]+s)};

	float t=(skewed[0][0]+skewed[0][1])*G2;
	unskewed[0]=(Vec2f){skewed[0][0]-t,skewed[0][1]-t};

	offset[0]=pos-unskewed[0];
	
	Vec2i delta;

	if(offset[0][0]>offset[0][1])
	{
		delta=(Vec2i){1,0};

	}
	else
	{
		delta=(Vec2i){0,1};
	}


	offset[1]=offset[0]-(Vec2f){delta[0],delta[1]}+(Vec2f){G2,G2};
	skewed[1]=skewed[0]+delta;

	offset[2]=offset[0]+(Vec2f){2*G2-1,2*G2-1};
	skewed[2]=skewed[0]+(Vec2i){1,1};

	float v=0.0;

	//int i=1;

	for(int i=0;i<3;i++)
	{
		float t=0.5-offset[i][0]*offset[i][0]-offset[i][1]*offset[i][1];

		if(t>0)
		{
			int g = noise->perm[(skewed[i][0]+noise->perm[skewed[i][1]&0xff])&0xff] % 12;

			v+=t*t*t*t*dot(grad2[g],offset[i]);

		}

	}

	return 70*(v);

}


