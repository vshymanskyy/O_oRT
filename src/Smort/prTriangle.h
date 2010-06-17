#pragma once
#include "prBase.h"

class prTriangle
	: public prBase
{

public:
	prTriangle(
		const vec3& A, const vec3& B, const vec3& C,
		const vec2& mA, const vec2& mB, const vec2& mC,
		//const vec3& nA, const vec3& nB, const vec3& nC,
		const shBase* MatFront, const shBase* MatBack
	);

	void Intersect(Ray &r) const;
	bool Hits(const Ray &r) const;
	void Shade(Ray &r, rgba* result) const;

	aabb GetAabb() const;

private:
	// -- representation
	const vec3 mPointA, mPointB, mPointC;
	const vec2 mTexA, mTexB, mTexC;
	//const vec3 nNormA, mNormB, mNormC;

	const shBase*	mMaterialFront;
	const shBase*	mMaterialBack;
	// -- end representation

	vec3 edge1, edge2, normal, tangent, bitangent;
	vec2 edge1uv, edge2uv;
	real nu, nv, nd;
	real bnu, bnv;
	real cnu, cnv;
	int k;
};
