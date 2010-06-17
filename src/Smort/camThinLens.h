#pragma once
#include "camBase.h"
#include "basis.h"
#include "vec3.h"
#include "vec2.h"

class camThinLens
	: public camBase
{

public:
	vec3 mPosition;
	vec3 mDirection;
	vec3 mUp;
	vec2 mViewSize;
	real mDistance;


	real mDofDistance;
	real mDofAperture;
	bool mUseDoF;

	basis mBase;


	real sample_step, half_aperture, half_sample_step, sqr_sample_step;
	vec3 dl, dr, ul, ur, dx, dy, unitX, unitY;

	camThinLens(const vec3 &Pos, const vec3 &Dir, const vec3 &Up, const real D, real vW, real vH);

	void UpdateBasis();
	void Precompute();
/*
	void SetResolution(int w, int h) {
		camBase::SetResolution(w, h);

		real b(w);
		real a(b * mViewSize.y);

		mViewSize.x = real(w);
		mViewSize.x *= mViewSize.y;
		mViewSize.x /= real(h);
	}
*/
	void getRay(real x, real y, Ray* result) const;
	void SaveSkel();

};
