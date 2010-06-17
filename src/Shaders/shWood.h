#pragma once
#include "ngBase.h"

class shWood
	: public ngBase
{

private:
	ngBase* mNoise;
	real mShift;
	real mScale;
	real mTurbulence;

public:
	shWood(ngBase* noise, real shift = 1.5, real scale = 2.0, real turbulence = 5.0);
	real Noise3D(const vec3& vec)const;
};
