#pragma once
#include "shBase.h"

class shRefract
	: public shBase
{

private:
	const shBase* mValue;
	const shBase* mCoeficient;


public:
	shRefract(const shBase* transm, const shBase* coef);
	void Shade(const Ray &r, rgba* result) const;
};
