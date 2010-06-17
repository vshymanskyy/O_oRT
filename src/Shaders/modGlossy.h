#pragma once
#include "shBase.h"
#include "cRandom.h"

class modGlossy
	: public modBase
{

private:
	const shBase* mData;

	void Modify(Ray &r) const {
		rgba r2;
		mData->Shade(r, &r2);
		const real gloss = r2.Luminance();
		if (gloss > real(0.00001)) {
			const real x = cRandom::InRange(-gloss, gloss);
			const real t = (Square(gloss) - Square(x)).Sqrt();
			const real y = cRandom::InRange(-t, t);

			const real theta = real::ATan2(r.gState.normal.y, r.gState.normal.x) + x;
			const real phi = r.gState.normal.z.ACos() + y;

			const real cp = phi.Cos();
			const real sp = phi.Sin();

			vec3 n(theta.Cos() * sp, theta.Sin() * sp, cp);

			if (r.gState.normal.dot(n) < real(0.0)) {
				r.gState.normal = -n;
			} else {
				r.gState.normal = n;
			}
			r.gState.cosND = -r.gState.normal.dot(r.dir);
		}
	}

public:
	modGlossy(const shBase* shader, const shBase* data)
		: modBase(shader)
		, mData(data)
	{}

};
