#pragma once
#include "shBase.h"

class shSub
	: public shBase
{
public:
	shSub(const shBase* sh1, const shBase* sh2)
		: mShader1(sh1)
		, mShader2(sh2)
	{}

	void Shade (const Ray &r, rgba* result) const {
		rgba r2;
		mShader1->Shade(r, result);
		mShader2->Shade(r, &r2);
		*result -= r2;
	}
private:
	const shBase* mShader1;
	const shBase* mShader2;
};
