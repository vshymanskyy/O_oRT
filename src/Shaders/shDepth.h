#pragma once
#include "shBase.h"

class shDepth
	: public shBase
{
public:
	void Shade(const Ray &r, rgba* result) const;
};

void shDepth::Shade(const Ray &r, rgba* result) const {
	*result = rgba(r.dist);
}
