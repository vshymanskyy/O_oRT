#include "shDiffuse.h"

#include "cRandom.h"

shDiffuse::shDiffuse(const shBase* color, real ray_length, int samples)
	: mColor(color)
	, mSamples(samples)
	, mRayLength(ray_length)
{
}

rgba shDiffuse::Shade(Ray &r) const{
	rgba Kd = mColor->Shade(r);
	rgba accum(0.0);
	if (Kd.Luminance() > 0.001) {
		for(int i=0; i<mSamples; i++) {
			const vec3 dir = cRandom::RandomVec3();
			const real dot = -r.gState.Normal.dot(dir);
			Ray r2(r, -dir, mRayLength);
			if (dot < cMath::Zero) {
				r2.Direction.invert();
				accum -= r.Scene->Trace(r2) * dot;
			} else {
				accum += r.Scene->Trace(r2) * dot;
			}
		}
	}
	return Kd*accum/float(mSamples);
}
