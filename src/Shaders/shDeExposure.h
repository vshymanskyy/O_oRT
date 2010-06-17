#pragma once
#include "shBase.h"

class shDeExposure
	: public shBase
{

public:
	shDeExposure(shBase *sh, real exposure);

	void Shade(const Ray &r, rgba* result) const;

private:
	const shBase *mShader;
	const real mExposure;
};
