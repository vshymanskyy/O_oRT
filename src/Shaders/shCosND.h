#pragma once
#include "shBase.h"

class shCosND: public shBase{
public:
	void Shade(const Ray &r, rgba* result) const {
		*result = rgba(r.gState.cosND);
	}
};
