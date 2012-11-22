#pragma once
#include "shBase.h"

class shReflect
	: public shBase
{

private:
	const shBase* mCoeficient;

public:
	shReflect(const shBase* coef)
		: mCoeficient(coef)
	{
	}

	void Shade(const Ray &r, rgba* result) const {
		rgba Kr;
		mCoeficient->Shade(r, &Kr);
		if (Kr.Luminance() > channel(0.0)) {
			const vec3 reflDir = r.dir + r.gState.normal * r.gState.cosND * real(2.0);
			Ray rayRefl(r.scene, r.gState.point, reflDir, r.cState.depth-1, r.parent);
			rayRefl.cState.refrN = r.cState.refrN;
			r.scene->Trace(rayRefl, result);
			*result *= Kr;
		} else {
			*result = rgba(0.0);
		}
	}
};
