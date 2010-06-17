#pragma once
#include "shBase.h"

class shAbs
	: public shBase
{
public:
	shAbs (const shBase *sh)
		: mShader(sh)
	{}

	void Shade (const Ray &r, rgba* result) const {
		mShader->Shade(r, result);
		*result = result->Abs();
	}
private:
	const shBase *mShader;
};
