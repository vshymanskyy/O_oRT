#pragma once
#include "Ray.h"
#include "HdrImage.h"
#include "rgba.h"
#include "precomputable.h"

class camBase
	: public precomputable
{
public:
	virtual ~camBase() {}
	virtual void getRay(real x, real y, Ray* result) const = 0;

	void setFrameSize(int w, int h) {
		mFrameWidth = w;
		mFrameHeight = h;
		Precompute();
	}
protected:
	int mFrameWidth;
	int mFrameHeight;
};
