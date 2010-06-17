#pragma once
#include "envBase.h"

class envCubic
	: public envBase
{

public:
	envCubic(shBase* up, shBase* down, shBase* right, shBase* left, shBase* front, shBase* back);

	void Shade(Ray &r, rgba* result) const;

private:
	shBase* mSides[6];

	enum CubeFace{
		cfUp = 0,
		cfDown = 1,
		cfRight = 2,
		cfLeft = 3,
		cfFront = 4,
		cfBack = 5
	};
};
