#pragma once
#include "modBase.h"
#include "basis.h"

class modNormal
	: public modBase
{

private:
	const shBase* mData;

	void Modify(Ray &r) const {
		rgba norm;
		mData->Shade(r, &norm);
		r.gState.normal = basis(r.gState.partialU,r.gState.normal, r.gState.partialV).transform(vec3(norm.r,norm.g,-norm.b) * real(2) - real(1));
	}

public:
	modNormal(const shBase* shader, const shBase* data)
		: modBase(shader)
		, mData(data)
	{}

};
