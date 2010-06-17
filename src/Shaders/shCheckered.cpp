#include "shCheckered.h"

shCheckered::shCheckered(real xW)
	: ngBase(1)
	, wX(xW)
	, wY(real(0))
	, wZ(real(0))
{
}

shCheckered::shCheckered(real xW, real yW)
	: ngBase(2)
	, wX(xW)
	, wY(yW)
	, wZ(real(0))
{
}

shCheckered::shCheckered(real xW, real yW, real zW)
	: ngBase(3)
	, wX(xW)
	, wY(yW)
	, wZ(zW)
{
}

bool shCheckered::InOddSquare(real x) const{
	bool ans = false;
	//	X
	if (x<real(0)) {
		ans = true;
		x = -x;
	}
	ans ^= (((long)(int)(x/wX))&0x01);
	return ans;
}

bool shCheckered::InOddSquare(real x, real y) const{
	bool ans = false;
	//	X
	if (x<real(0)) {
		ans = true;
		x = -x;
	}
	ans ^= (((long)(int)(x/wX))&0x01);
	//	Y
	if (y<real(0)) {
		ans = !ans;
		y = -y;
	}
	ans ^= (((long)(int)(y/wY))&0x01);
	return ans;
}

bool shCheckered::InOddSquare(real x, real y, real z) const{
	bool ans = false;
	//	X
	if (x<real(0)) {
		ans = true;
		x = -x;
	}
	ans ^= (((long)(int)(x/wX))&0x01);
	//	Y
	if (y<real(0)) {
		ans = !ans;
		y = -y;
	}
	ans ^= (((long)(int)(y/wY))&0x01);
	//	Z
	if (z<real(0)) {
		ans = !ans;
		z = -z;
	}
	ans ^= (((long)(int)(z/wZ))&0x01);
	return ans;
}

real shCheckered::Noise1D(const real& val) const{
	return InOddSquare(val)? real(1) : real(0);
}

real shCheckered::Noise2D(const vec2& vec)const{
	return InOddSquare(vec.x, vec.y)? real(1) : real(0);
}

real shCheckered::Noise3D(const vec3& vec)const{
	return InOddSquare(vec.x, vec.y, vec.z)? real(1) : real(0);
}
