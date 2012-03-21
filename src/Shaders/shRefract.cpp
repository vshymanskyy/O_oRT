#include "shRefract.h"
#include "cRandom.h"

shRefract::shRefract(const shBase* transm, const shBase* coef)
	: mValue(transm)
	, mCoeficient(coef)
{
}

void
shRefract::Shade(const Ray &r, rgba* result) const
{
	rgba Kr;
	mValue->Shade(r, &Kr);
	if (Kr.Luminance() != channel(0.0)) {
		rgba N;
		mCoeficient->Shade(r, &N);

		const real n1 = r.cState.refrN;
		const real n2 = N.Luminance();

		const real n = (r.gState.inside)?(n2/n1):(n1/n2);

		real sin;
		if (n >= real(0.99) && n <= real(0.001)) {
			sin = real(100.0);
		} else {
			sin = real(1.0) - Square(n) * (real(1.0) - Square(r.gState.cosND));
		}


		vec3 refrDir;
		if (n >= real(0.99) && n <= real(0.001)) {
			refrDir = r.dir;
		} else {
			refrDir = r.dir * n + r.gState.normal * (r.gState.cosND * n - sin.Sqrt());
		}


		Ray rayRefr(r.scene, r.gState.point, refrDir, r.cState.depth-1, (r.gState.inside) ? r.parent : &r);
		rayRefr.cState.refrN = n2;
		r.scene->Trace(rayRefr, result);
		if(!r.gState.inside) {
			*result *= Kr;
		}

	} else {
		*result = rgba(0.0);
	}
}
