#include "fltGaussian.h"

fltGaussian::fltGaussian(const vec2& size, real a)
	: fltBase(size)
	, alpha(a)
{
	expX = expf(-alpha * Square(mSize.x));
	expY = expf(-alpha * Square(mSize.y));
}

fltGaussian::fltGaussian(real a)
	: alpha(a)
{
	expX = expf(-alpha * Square(mSize.x));
	expY = expf(-alpha * Square(mSize.y));
}

real fltGaussian::Gaussian(real d, real expv) const{
	return (-alpha * d * d).Exp() - expv;
}

real fltGaussian::getWeight(const vec2 &pos) const {
	return Gaussian(pos.x, expX) * Gaussian(pos.y, expY);
}

