#pragma once
#include "shBase.h"

class shPartialV: public shBase{

public:
	void Shade(const Ray &r, rgba* result) const {
		*result = rgba(
			 r.gState.partialV.x.Abs()
			,r.gState.partialV.y.Abs()
			,r.gState.partialV.z.Abs()
		);
	}
};
