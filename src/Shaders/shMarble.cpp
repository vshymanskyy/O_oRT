#include "shMarble.h"

shMarble::shMarble(ngBase* noise, real shift /*= 1.5*/, real scale /*= 2.0*/, real turbulence /*= 5.0*/)
	: ngBase(noise->GetDimensions())
	, mNoise(noise)
	, mShift(shift)
	, mScale(scale)
	, mTurbulence(turbulence)
{
}

real shMarble::Noise3D(const vec3& vec) const{
	const real v = mNoise->Noise3D(vec);
	return mNoise->Noise3D(vec3(vec.x + mScale*(mTurbulence*v).Sin(), vec.y+mScale*(mTurbulence*v).Cos(), vec.z));
}
