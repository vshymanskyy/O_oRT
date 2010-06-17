#include "shTurbulence.h"

real shTurbulence::Noise1D(const real& v) const{
	real val = v;
	real
		result = real(0),
		amp = mAmplitude;

	val *= mFrequency;

	for (int i=0; i<mOctaves; i++) {
		result += mNoise->Noise1D(val) * amp;
		val *= 2.0f;
		amp *= 0.5f;
	}
	return result;
}

real shTurbulence::Noise2D(const vec2& v) const{
	vec2 vec = v;
	real
		result = real(0),
		amp = mAmplitude;

	vec *= mFrequency;

	for (int i=0; i<mOctaves; i++) {
		result += mNoise->Noise2D(vec) * amp;
		vec *= real(2);
		amp *= real(0.5);
	}
	return result;
}

real shTurbulence::Noise3D(const vec3& v) const{
	vec3 vec = v;
	real result = real(0);
	real amp = mAmplitude;

	vec *= mFrequency;

	for (int i=0; i<mOctaves; i++) {
		result += mNoise->Noise3D(vec) * amp;
		vec *= real(2);
		amp *= real(0.5);
	}
	return result;
}

shTurbulence::shTurbulence(ngBase* noise, int octaves, real freq, real amp)
	:ngBase(noise->GetDimensions())
	,mNoise(noise)
	,mOctaves(octaves)
	,mFrequency(freq)
	,mAmplitude(amp)
{
}
