#pragma once
#include "ngBase.h"

#define PerlinSampleSize	128

class shPerlin
	: public ngBase
{
private:
	int  p [2 * PerlinSampleSize + 2];
	real g1[2 * PerlinSampleSize + 2];
	real g2[2 * PerlinSampleSize + 2][2];
	real g3[2 * PerlinSampleSize + 2][3];

	real noise1(real arg)const;
	real noise2(real vec[2])const;
	real noise3(real vec[3])const;

public:
	shPerlin(int dimensions = 3, int seed = 0);

	real Noise1D(const real& val)const;
	real Noise2D(const vec2& vec)const;
	real Noise3D(const vec3& vec)const;
};
