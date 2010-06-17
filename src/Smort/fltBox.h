#pragma once
#include "fltBase.h"

class fltBox
	: public fltBase
{
public:
	fltBox() {}

	real getWeight(const vec2 &pos) const {
		return real(1.0);
	}
};
