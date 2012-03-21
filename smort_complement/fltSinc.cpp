#include "fltSinc.h"

fltSinc::fltSinc(real t)
	: tau(t)
{
}

real fltSinc::Sinc1D(real x) const {
	x = x.Abs();
	if (x < cMath::Tiny) {
		return real(1.0);
	}
	if (x > real(1.0)) {
		return real(0.0);
	}
	x *= cMath::PI;
	const real sinc = (x * tau).Sin() / (x * tau);
	const real lanczos = x.Sin() / x;
	return sinc * lanczos;
}

real fltSinc::getWeight(const vec2 &pos) const{
	return Sinc1D(pos.x * mInvSize.x) * Sinc1D(pos.y * mInvSize.y);
}
