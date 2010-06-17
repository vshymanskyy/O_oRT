#pragma once
#include "fltBase.h"

class fltSinc : public fltBase {
private:
	real tau;
	real Sinc1D(real x) const;
public:
	fltSinc(real t);
	real getWeight(const vec2 &pos) const;
};
