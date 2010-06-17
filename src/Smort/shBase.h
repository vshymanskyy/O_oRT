#pragma once
#include "Ray.h"
#include "rgba.h"
#include "prBase.h"

struct shBase {
	virtual ~shBase() {};
	virtual void Shade (const Ray &r, rgba* result) const = 0;
};
