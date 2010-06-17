#include "shDeGamma.h"

shDeGamma::shDeGamma(shBase *sh, real gamma)
	:mShader(sh)
	,mGamma(gamma)
{

}

void
shDeGamma::Shade(const Ray &r, rgba* result) const
{
	mShader->Shade(r, result);

	result->r = powf(result->r, mGamma);
	result->g = powf(result->g, mGamma);
	result->b = powf(result->b, mGamma);

}
