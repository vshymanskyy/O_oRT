#include "envStarSky.h"

envStarSky::envStarSky(real filled)
	: mFilled(filled)
{}

#define sqr(a)((a)*(a))

rgba envStarSky::Shade(Ray &r) const{
	return (fmod(fabs(r.Direction.x)*100+fabs(r.Direction.y)*10+fabs(r.Direction.z), 1)<mFilled)?(rgba(1,1,1)):(rgba(0,0,0));
}

