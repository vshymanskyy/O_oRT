#pragma once
#include "shBase.h"

class shNormal
	: public shBase
{
	void Shade(const Ray &r, rgba* result) const;
};

inline
void
shNormal::Shade(const Ray &r, rgba* result) const
{
	*result = rgba(
		 r.gState.normal.x
		,r.gState.normal.y
		,r.gState.normal.z
	);
}
