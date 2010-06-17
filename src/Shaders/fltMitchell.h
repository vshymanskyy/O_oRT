#pragma once
#include "fltBase.h"

class fltMitchell
	: public fltBase
{
private:
	real B, C;
	real Mitchell1D(real x) const;
public:
	fltMitchell(real b, real c);
	real getWeight(const vec2 &pos) const;
};
