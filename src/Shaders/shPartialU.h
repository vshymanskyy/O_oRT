#pragma once
#include "shBase.h"

class shPartialU: public shBase{

public:
	void Shade(const Ray &r, rgba* result) const {
		*result = rgba(
			 r.gState.partialU.x.Abs()
			,r.gState.partialU.y.Abs()
			,r.gState.partialU.z.Abs()
		);
	}
};
