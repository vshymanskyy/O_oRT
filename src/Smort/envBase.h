#pragma once
#include "rgba.h"
#include "Ray.h"
#include "shBase.h"

struct envBase {
	virtual ~envBase() {};
	virtual void Shade (Ray &r, rgba* result) const = 0;
};
