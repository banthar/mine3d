
#ifndef _MATH_H_
#define _MATH_H_

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

typedef int vec2i __attribute__ ((vector_size (2*sizeof(int))));
typedef int vec4i __attribute__ ((vector_size (4*sizeof(int))));
typedef float vec2f __attribute__ ((vector_size (2*sizeof(float))));
typedef float vec4f __attribute__ ((vector_size (4*sizeof(float))));

#endif
