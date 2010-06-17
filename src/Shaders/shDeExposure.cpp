#include "shDeExposure.h"

shDeExposure::shDeExposure(shBase *sh, real exposure)
	:mShader(sh)
	,mExposure(exposure)
{
}

void
shDeExposure::Shade(const Ray &r, rgba* result) const
{
	mShader->Shade(r, result);

	result->r = -logf(1.001f - result->r);
	result->g = -logf(1.001f - result->g);
	result->b = -logf(1.001f - result->b);

	*result *= mExposure;
}
