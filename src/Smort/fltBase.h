#pragma once
#include "vec2.h"

class fltBase {

public:
	fltBase();
	fltBase(const vec2& size);

	virtual real getWeight(const vec2& pos) const = 0;

protected:
	vec2 mSize;
	vec2 mInvSize;

};

inline
fltBase::fltBase()
	: mSize(real(1.0), real(1.0))
	, mInvSize(real(1.0), real(1.0))
{
}

inline
fltBase::fltBase(const vec2 &size)
	: mSize(size)
	, mInvSize(vec2(real(1.0), real(1.0)) / size)
{
}
