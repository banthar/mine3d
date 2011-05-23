
#include "snoise3.h"

#include <xmmintrin.h>
#include <emmintrin.h>

#include <stdint.h>

typedef Vec4f v4sf;
typedef Vec4i v4si;

struct Noise
{
	uint8_t perm[256];
	uint8_t mperm[256]; // perm mod 12
};

#define LET(A, B) typeof(B) A = B

#define vec4f(A, B, C, D) ((v4sf) _mm_set_ps(D, C, B, A))

#define vec1_4f(S) ((v4sf) _mm_set1_ps(S))

#define isum(V) ({ int i[4]; *(typeof(V)*) &i = V; i[0] + i[1] + i[2]; })

#define sum3(V) ({ float f[4]; *(typeof(V)*) &f = V; f[0] + f[1] + f[2]; })
#define sum4(V) ({ float f[4]; *(typeof(V)*) &f = V; f[0] + f[1] + f[2] + f[3]; })

Noise* newNoise(uint64_t seed)
{

	Noise* nc=malloc(sizeof(Noise));
	
	unsigned int s=seed;

	for(int i = 0; i < 256; ++i)
	{
		nc->perm[i] = rand_r(&s);
		nc->mperm[i] = nc->perm[i] % 12;
	}
  
	return nc;

}

float noise3(const Noise *nc, v4sf v)
{

	const static int offsets[8][2][4] = {
		{{1, 0, 0, 0}, {1, 1, 0, 0}},
		{{0, 1, 0, 0}, {1, 1, 0, 0}},
		{{0, 0, 0, 0}, {0, 0, 0, 0}},
		{{0, 1, 0, 0}, {0, 1, 1, 0}},
		{{1, 0, 0, 0}, {1, 0, 1, 0}},
		{{0, 0, 0, 0}, {0, 0, 0, 0}},
		{{0, 0, 1, 0}, {1, 0, 1, 0}},
		{{0, 0, 1, 0}, {0, 1, 1, 0}}
	};

	v4sf vs[4], vsum;
	int gi[4], mask, c;
	v4si indices;

	vsum = v + vec1_4f(sum3(v) / 3);
	indices = __builtin_ia32_psubd128 (__builtin_ia32_cvttps2dq(vsum), __builtin_ia32_psrldi128 ((v4si) vsum, 31));
	vs[0] = v - __builtin_ia32_cvtdq2ps(indices) + vec1_4f(isum(indices) / 6.0f);
	vs[1] = vs[0] + vec1_4f(     1.0f/6.0f);
	vs[2] = vs[0] + vec1_4f(     2.0f/6.0f);
	vs[3] = vs[0] + vec1_4f(-1.0f + 3.0f/6.0f);
	v4sf xxy = __builtin_ia32_shufps(vs[0], vs[0], _MM_SHUFFLE(0, 1, 0, 0));
	v4sf yzz = __builtin_ia32_shufps(vs[0], vs[0], _MM_SHUFFLE(0, 2, 2, 1));
	mask = __builtin_ia32_movmskps(__builtin_ia32_cmpltps(xxy, yzz));
	LET(opp, &offsets[mask & 7]);
	#define op (*opp)
	#define offs1 (op[0])
	#define offs2 (op[1])
	vs[1] -= __builtin_ia32_cvtdq2ps(*(v4si*)&offs1);
	vs[2] -= __builtin_ia32_cvtdq2ps(*(v4si*)&offs2);
	int indexfield[4]; *(typeof(indices)*) indexfield = indices;
	#define ii indexfield[0]
	#define jj indexfield[1]
	#define kk indexfield[2]
	#define i1 offs1[0]
	#define i2 offs2[0]
	#define j1 offs1[1]
	#define j2 offs2[1]
	#define k1 offs1[2]
	#define k2 offs2[2]

	unsigned const char* mperm=nc->mperm;
	unsigned const char* perm=nc->perm;

	gi[0] = mperm[(perm[(perm[(kk   )&0xff]+jj   )&0xff]+ii   )&0xff];
	gi[1] = mperm[(perm[(perm[(kk+k1)&0xff]+jj+j1)&0xff]+ii+i1)&0xff];
	gi[2] = mperm[(perm[(perm[(kk+k2)&0xff]+jj+j2)&0xff]+ii+i2)&0xff];
	gi[3] = mperm[(perm[(perm[(kk+1 )&0xff]+jj+1 )&0xff]+ii+1 )&0xff];
	float factors[4];
	float pair[3], res[4];
	pair[0] = 1; pair[1] = -1; pair[2] = -1;
	for (c = 0; c < 4; ++c) {
	LET(vscp, &(vs[c]));
	LET(current, *vscp);
	{
	  LET(A, current * current);
	  LET(B, __builtin_ia32_shufps(A, A, _MM_SHUFFLE(1, 1, 1, 1)));
	  LET(C, __builtin_ia32_shufps(A, A, _MM_SHUFFLE(2, 2, 2, 2)));
	  LET(D, A + B + C);
	  LET(E, vec1_4f(0.6f) - D);
	  factors[c] = *(float*) &E;
	}
	if (factors[c] >= 0) {
	  int id = gi[c];
	  res[c] = (((float*)vscp)[id >> 3] * pair[id & 1]) + (((float*)vscp)[(((id >> 2) | (id >> 3)) & 1) + 1] * pair[id&2]);
	} else {
	  factors[c] = 0;
	  res[c] = 0;
	}
	}
	v4sf vfactors = vec4f(factors[0], factors[1], factors[2], factors[3]);
	vfactors *= vfactors;
	vfactors *= vfactors;
	v4sf vres = vec4f(res[0], res[1], res[2], res[3]);
	vres *= vfactors;
	return 0.5f + 16 * sum4(vres);
}

