#include "shWood.h"

shWood::shWood(ngBase* noise, real shift /*= 1.5*/, real scale /*= 2.0*/, real turbulence /*= 5.0*/)
	: ngBase(noise->GetDimensions())
	, mNoise(noise)
	, mShift(shift*real(30.0))
	, mScale(scale)
	, mTurbulence(turbulence)
{
}

real shWood::Noise3D(const vec3& vec) const{
	const real
		xx = vec.x+mTurbulence*mNoise->Noise3D(vec),
		yy = vec.y+mTurbulence*mNoise->Noise3D(vec3(vec.x+mShift, vec.y, vec.z));
	const real res = real(1)/mScale*(Square(xx)+Square(yy).Sqrt());
	return res.Mod(real(1));
}
