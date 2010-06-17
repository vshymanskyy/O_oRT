#include "shMirror.h"

shMirror::shMirror(const shBase* data)
	: mReflectance(data)
{
}

void
shMirror::Shade(const Ray &r, rgba* result) const
{
	rgba Kr;
	mReflectance->Shade(r, &Kr);
	if (Kr.Luminance() > channel(0.0)) {
		const vec3 reflDir = r.dir + r.gState.normal * r.gState.cosND * real(2.0);
		Ray rayRefl(r.scene, r.gState.point, reflDir, r.cState.depth-1, r.parent);
		rayRefl.cState.refrN = r.cState.refrN;
		r.scene->Trace(rayRefl, result);

		const real cos = real(1.0) - r.gState.cosND;
		const real cos2 = cos * cos;
		const real cos5 = cos2 * cos2 * cos;
		*result *= Kr + (rgba(1.0) - Kr) * cos5;
	} else {
		*result = rgba(0.0);
	}
}
