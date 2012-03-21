#pragma once
#include "prBase.h"

class prSphere
	: public prBase
{

public:
	prSphere(const vec3 center, real radius, const shBase* MatFront, const shBase* MatBack = NULL);

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

	const shBase*	mMaterialFront;
	const shBase*	mMaterialBack;
	// -- end representation

	const vec3 mNcrossE;
	const real mSqrRadius;
};
