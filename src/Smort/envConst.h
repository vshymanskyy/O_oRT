#pragma once
#include "envBase.h"

class envConst
	: public envBase
{

public:
	envConst(const rgba& color)
		: mColor(color)
	{}

	void Shade(Ray &r, rgba* result) const{
		r.dist = real(0.1);
		r.gState.normal = vec3(0.0, 0.0, 0.0);
		r.UpdatePoint();
		*result = mColor;
	}

private:
	const rgba mColor;
};
