#pragma once
#include "modBase.h"

class modAffineUV: public modBase{

private:
	vec2 m1, m2, m3;

	void Modify(Ray &r) const;

public:
	modAffineUV(const shBase* shader);

	void SetScaling(real ku, real kv);
	void SetLinearMatrix(real a11, real a21, real a12, real a22);
	void SetAffineMatrix(real a11, real a21, real a12, real a22, real a13, real a23);
	void SetTriangleCoords(const vec2& a, const vec2& b, const vec2& c);

};
