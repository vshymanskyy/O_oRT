#pragma once
#include "envBase.h"

class envStarSky: public envBase{

public:
	envStarSky(real filled);
	rgba Shade(Ray &r) const;

private:
	real mFilled;

};
