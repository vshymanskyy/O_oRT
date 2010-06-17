#pragma once
#include "prBase.h"

class prSphere
	: public prBase
{

public:
	prSphere(const vec3 center, real radius, shBase *mat, bool frontFace = true, bool backFace = true);

	void Intersect(Ray &r) const;
	bool Hits(const Ray &r) const;
	void Shade(Ray &r, rgba* result) const;

	aabb GetAabb() const;

private:
	// -- representation
	const vec3		mCenter;
	const vec3		mNorth;
	const vec3		mEquador;
	const real		mRadius;

	const shBase*	mMaterial;
	const bool		mFrontFace;
	const bool		mBackFace;
	// -- end representation

	const vec3 mNcrossE;
	const real mSqrRadius;
};
