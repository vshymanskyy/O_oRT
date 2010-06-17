#pragma once
#include "modBase.h"

class modBilinearUV
	: public modBase
{

private:
	vec2 mA, mB, mC, mD;

	void Modify(Ray &r) const;

public:
	modBilinearUV(const shBase* shader, const vec2& a, const vec2& b, const vec2& c, const vec2& d);

};
