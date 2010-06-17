#pragma once
#include "fltBase.h"

class fltGaussian
	: public fltBase
{
private:
	real alpha;
	real expX, expY;
	real Gaussian(real d, real expv) const;

public:
	fltGaussian(real a);
	fltGaussian(const vec2& size, real a);
	real getWeight(const vec2 &pos) const;
};
