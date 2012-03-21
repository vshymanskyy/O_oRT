#pragma once
#include "Ray.h"
#include "rgba.h"
#include "prBase.h"

struct shBase {
	virtual ~shBase() {};
	virtual void Shade (const Ray &r, rgba* result) const = 0;
};


class shConst
	: public shBase
{

public:
	shConst(const rgba& c)
		: mColor(c)
	{}

	shConst(const channel& gray)
		: mColor(gray)
	{}

	shConst(const channel R, const channel G, const channel B, const channel A = 1.0)
		: mColor(R, G, B, A)
	{}

	void Shade (const Ray &r, rgba* result) const {
		*result = mColor;
	}

private:
	const rgba mColor;
};


template <const double & R, const double & G, const double & B>
class tshConst
{

public:
	static rgba Shade (const Ray &r) {
		return rgba(R, G, B);
	}
};

template <typename T>
class tshConst2
{

public:
	static rgba Shade (const Ray &r) {
		return T::Shade(r) ;
	}
};
