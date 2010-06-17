#pragma once
#include "shBase.h"

class shBlend
	: public shBase
{
private:
	const shBase *mShader1, *mShader2, *mAlpha;

public:
	shBlend(shBase *sh1, shBase *sh2, shBase *alpha)
		: mShader1(sh1)
		, mShader2(sh2)
		, mAlpha(alpha)
	{}

	void Shade(const Ray &r, rgba* result) const {
		rgba r2, a;
		mAlpha->Shade(r, &a);
		if(a.Luminance() <= channel(0.0001)) {
			mShader1->Shade(r, result);
		} else if(a.Luminance() >= channel(0.999)) {
			mShader2->Shade(r, result);
		} else {
			mShader1->Shade(r, result);
			mShader2->Shade(r, &r2);
			result->Blend(r2, a);
		}
	}
};
