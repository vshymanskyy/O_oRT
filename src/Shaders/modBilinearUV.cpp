#include "modBilinearUV.h"

modBilinearUV::modBilinearUV(const shBase* shader, const vec2& a, const vec2& b, const vec2& c, const vec2& d)
	: modBase(shader)
{
	mA = a;
	mB = b;
	mC = c;
	mD = d;
}

void
modBilinearUV::Modify(Ray &r) const
{
	real uCoord = r.gState.textureUV.x;
	real vCoord = r.gState.textureUV.y;
	r.gState.textureUV =
			(
				mA * (real(1) - vCoord)
				+
				mD * vCoord
			)
			*
			(real(1) - uCoord)
		+
			(
				mB * (real(1) - vCoord)
				+
				mC * vCoord
			)
			*
			uCoord
		;
}
