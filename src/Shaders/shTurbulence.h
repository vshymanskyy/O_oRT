#pragma once
#include "ngBase.h"

class shTurbulence : public ngBase{

private:
	ngBase* mNoise;
	int  mOctaves;
	real mFrequency;
	real mAmplitude;

public:
	shTurbulence(ngBase* noise, int octaves = 4, real freq = 6.0, real amp = 1.0);

	real Noise1D(const real& v) const;
	real Noise2D(const vec2& v) const;
	real Noise3D(const vec3& v) const;
};
