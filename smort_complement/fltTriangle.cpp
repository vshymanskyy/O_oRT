#include "fltTriangle.h"

fltTriangle::fltTriangle() {
}

real fltTriangle::getWeight(const vec2 &pos) const {
	return (mSize.x - pos.x.Abs())/mSize.x * (mSize.y - pos.y.Abs())/mSize.y;
}
