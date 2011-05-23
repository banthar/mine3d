
#pragma once

#include <stdint.h>

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)>(b)?(b):(a))

static inline double clampf(double v, double min, double max)
{
	if(v<min)
		return min;
	else if(v>max)
		return max;
	else
		return v;
}

typedef int Vec2i __attribute__ ((vector_size (2*sizeof(int))));
typedef int Vec4i __attribute__ ((vector_size (4*sizeof(int))));
typedef float Vec2f __attribute__ ((vector_size (2*sizeof(float))));
typedef float Vec4f __attribute__ ((vector_size (4*sizeof(float))));
typedef uint8_t Vec2b __attribute__ ((vector_size (2*sizeof(uint8_t))));
typedef uint8_t Vec4b __attribute__ ((vector_size (4*sizeof(uint8_t))));


