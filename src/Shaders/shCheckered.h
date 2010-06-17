#pragma once
#include "ngBase.h"

class shCheckered : public ngBase{
private:
	const real wX, wY, wZ;

	bool InOddSquare(real d) const;
	bool InOddSquare(real u, real v) const;
	bool InOddSquare(real x, real y, real z) const;

public:
	shCheckered(real xW);
	shCheckered(real xW, real yW);
	shCheckered(real xW, real yW, real zW);

	real Noise1D(const real& vec)const;
	real Noise2D(const vec2& vec)const;
	real Noise3D(const vec3& vec)const;
};
