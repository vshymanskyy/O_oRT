#pragma once
#include "fltBase.h"

class fltTriangle : public fltBase {
public:
	fltTriangle();
	real getWeight(const vec2 &pos) const;
};
