#include "shGlass.h"
#include "cRandom.h"

shGlass::shGlass(const shBase* transm, const shBase* coef, const shBase* abs, const shBase* abs_dist)
	: mValue(transm)
	, mCoeficient(coef)
	, mAbsorbtion(abs)
	, mAbsorbtionDistance(abs_dist)
	, mPolarization(S_POLARIZED)
{
}

void
shGlass::Shade(const Ray &r, rgba* result) const
{
	rgba Kr;
	mValue->Shade(r, &Kr);
	if (Kr.Luminance() != channel(0.0)) {

		const real n1 = r.cState.refrN;
		real n2;
		if (r.gState.inside) {
			n2 = (r.parent) ? r.parent->cState.refrN : real(1.0);
		} else {
			rgba N;
			mCoeficient->Shade(r, &N);
			n2 = N.Luminance();
		}

		const int chromIdx = cRandom::InRange(1, 4);
		const real chromN[4] = { 0.0, -(n2-real(1.0))*real(0.05), 0.0, (n2-real(1.0))*real(0.05) };
		const rgba chromCol[4] = { rgba(1.0, 1.0, 1.0), rgba(1.6, 0.7, 0.7), rgba(0.7, 1.6, 0.7), rgba(0.7, 0.7, 1.6) };

		const real n = n1/(n2 + chromN[chromIdx]);

		real sin;
		if (n == real(1.0)) {
			sin = real(100.0);
		} else {
			sin = real(1.0) - Square(n) * (real(1.0) - Square(r.gState.cosND));
		}

		if (sin > real(0.0)) {
			vec3 refrDir;
			if (n == real(1.0)) {
				refrDir = r.dir;
			} else {
				refrDir = r.dir * n + r.gState.normal * (r.gState.cosND * n - sin.Sqrt());
			}

			//if (mPolarization == S_POLARIZED) {
				const real tmp1 = n1 * r.gState.cosND;
				const real tmp2 = -n2 * r.gState.normal.dot(refrDir);
			/*} else {
				const real tmp1 = -n1 * r.gState.normal.dot(refrDir);
				const real tmp2 = n2 * r.gState.cosND;
			}*/

			const real fresnel_coef = Square((tmp1 - tmp2)/(tmp1 + tmp2));
			if (fresnel_coef < real(0.99)) {
				Ray rayRefr(r.scene, r.gState.point, refrDir, r.cState.depth-1, (r.gState.inside) ? r.parent : &r);
				rayRefr.cState.refrN = n2;
				r.scene->Trace(rayRefr, result);
				*result *= real(1.0) - fresnel_coef;
				*result *= chromCol[chromIdx];
			} else {
				*result = rgba(0.0);
			}
			if (fresnel_coef > real(0.001)) {
				const vec3 reflDir = r.dir + r.gState.normal * r.gState.cosND * real(2.0);
				Ray rayRefl(r.scene, r.gState.point, reflDir, r.cState.depth-1, (r.gState.inside) ? &r : r.parent);
				rayRefl.cState.refrN = n1;
				rgba tmpRefl;
				r.scene->Trace(rayRefl, &tmpRefl);
				*result += tmpRefl * fresnel_coef;
			}
		} else {
			// Case of full reflection
			const vec3 reflDir = r.dir + r.gState.normal * r.gState.cosND * real(2.0);
			Ray rayRefl(r.scene, r.gState.point, reflDir, r.cState.depth-1, r.parent);
			rayRefl.cState.refrN = n1;
			rgba tmpRefl(0.0);
			r.scene->Trace(rayRefl, result);
		}

		if (r.gState.inside) {
			if (mAbsorbtion && mAbsorbtionDistance) {
				rgba absorption;
				rgba absorptionDist;
				mAbsorbtion->Shade(r, &absorption);
				mAbsorbtionDistance->Shade(r, &absorptionDist);
				absorption = absorption.Opposite();
				const real abs_dist = absorptionDist.Luminance();
				if (abs_dist > real(0)) {
					*result *= (absorption * (-(r.dist/abs_dist))).Exp();
				}
			}
		} else {
			*result *= Kr;
		}
	} else {
		*result = rgba(0.0);
	}
}
