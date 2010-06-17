#pragma once
#include "shBase.h"

class shMirror: public shBase{

private:
	const shBase* mReflectance;

public:
	shMirror(const shBase* data);
	void Shade(const Ray &r, rgba* result) const;
};
