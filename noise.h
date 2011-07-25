
#pragma once

#include "math.h"

typedef struct
{
	int perm[256];
}Noise;

public void noiseInit(Noise* noise, unsigned int seed);

public float noise2(const Noise* noise, Vec2f pos);
