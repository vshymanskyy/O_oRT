#pragma once
#include "envBase.h"

class envSpheric
	: public envBase
{

public:
	envSpheric(shBase* map);

	void Shade(Ray &r, rgba* result) const;

private:
	shBase* mMap;
};
