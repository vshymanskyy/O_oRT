#pragma once
#include "shBase.h"
#include "shBase.h"

class shDiffuse: public shBase{

private:
	const shBase* mColor;
	int mSamples;
	real mRayLength;

public:
	shDiffuse(const shBase* color, real ray_length = real(1.5), int samples = 5);
	rgba Shade(Ray &r) const;
};
