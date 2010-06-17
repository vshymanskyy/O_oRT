#pragma once
#include "shBase.h"

class shTrace: public shBase{

private:
	const shBase* mData;

public:
	shTrace(const shBase* data)
		: mData(data)
	{
	}

	void Shade (const Ray &r, rgba* result) const {
		mData->Shade(r, result);
		Ray r2(r.scene, r.gState.point, vec3(result->r, result->g, result->b), r.cState.depth-1, r.parent);
		r.scene->Trace(r2, result);
	}
};
