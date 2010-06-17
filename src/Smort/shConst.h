#pragma once
#include "shBase.h"

class shConst
	: public shBase
{

public:
	shConst(const rgba& c)
		: mColor(c)
	{}

	shConst(const channel& gray)
		: mColor(gray)
	{}

	shConst(const channel R, const channel G, const channel B, const channel A = 1.0)
		: mColor(R, G, B, A)
	{}

	void Shade (const Ray &r, rgba* result) const {
		*result = mColor;
	}

private:
	const rgba mColor;
};
