#pragma once
#include "shBase.h"

class shGlass
	: public shBase
{
	enum Polarization {
		S_POLARIZED,
		P_POLARIZED
	};

private:
	const shBase* mValue;
	const shBase* mCoeficient;
	const shBase* mAbsorbtion;
	const shBase* mAbsorbtionDistance;
	const Polarization mPolarization;

public:
	shGlass(const shBase* transm, const shBase* coef, const shBase* abs = NULL, const shBase* abs_dist = NULL);
	void Shade(const Ray &r, rgba* result) const;
};
