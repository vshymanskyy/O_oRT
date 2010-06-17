#include "modAffineUV.h"

modAffineUV::modAffineUV(const shBase* shader)
	: modBase(shader)
{
	m1.x = m2.y = real(1);
	m2.x = m1.y = m3.x = m3.y = real(0);
}

void
modAffineUV::Modify(Ray &r) const
{
	const real u = r.gState.textureUV.x;
	const real v = r.gState.textureUV.y;
	r.gState.textureUV = m1*u + m2*v + m3;
}

void
modAffineUV::SetScaling(real ku, real kv)
{
	m1.x = ku;
	m2.y = kv;
	m2.x = m1.y = m3.x = m3.y = real(0);
}

void
modAffineUV::SetLinearMatrix(real a11, real a21, real a12, real a22)
{
	SetAffineMatrix(a11, a21, a12, a22, real(0), real(0));
}

void
modAffineUV::SetAffineMatrix(real a11, real a21, real a12, real a22, real a13, real a23)
{
	m1.x = a11;
	m1.y = a21;
	m2.x = a12;
	m2.y = a22;
	m3.x = a13;
	m3.y = a23;
}

void
modAffineUV::SetTriangleCoords(const vec2& a, const vec2& b, const vec2& c)
{
	m1 = b - a;
	m2 = c - a;
	m3 = a;
}
