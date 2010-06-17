#include "fltMitchell.h"

fltMitchell::fltMitchell(real b, real c)
	: B(b)
	, C(c)
{
}

real fltMitchell::Mitchell1D(real x) const{
	x = (real(2.0) * x).Abs();
	if (x > real(1.0)) {
		return (((-B/real(6.0) - C) * x + (B + real(5.0)*C)) * x + (-real(2.0)*B - real(8.0)*C)) * x + (real(4.0)/real(3.0)*B + real(4.0)*C);
	} else {
		return ((real(2.0) - real(1.5)*B - C) * x + (-real(3.0) + real(2.0)*B + C)) * x*x + (real(1.0) - B/real(3.0));
	}
}

real fltMitchell::getWeight(const vec2 &pos) const {
	return Mitchell1D(pos.x * mInvSize.x) * Mitchell1D(pos.y * mInvSize.y);
}
