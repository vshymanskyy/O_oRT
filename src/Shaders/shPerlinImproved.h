#pragma once
#include "ngBase.h"

class shPerlinImproved: public ngBase{
private:

public:
	shPerlinImproved();

	real Noise3D(const vec3& vec)const;
};
