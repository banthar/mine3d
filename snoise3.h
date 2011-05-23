
#pragma once

#include "math.h"

#include <stdint.h>

struct Noise;

typedef struct Noise Noise;

Noise* newNoise(uint64_t seed);
float noise3(const Noise *noise, Vec4f vec);

