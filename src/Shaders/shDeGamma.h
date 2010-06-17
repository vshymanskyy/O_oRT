#pragma once
#include "shBase.h"

class shDeGamma: public shBase{

public:
	shDeGamma(shBase *sh, real gamma);
	void Shade(const Ray &r, rgba* result) const;

private:
	const shBase *mShader;
	const real mGamma;
};
