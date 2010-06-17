#pragma once
#include "shBase.h"

class modBase
	: public shBase
{
private:
	const shBase* mShader;

protected:
	virtual void Modify(Ray& r) const = 0;

public:
	modBase(const shBase* data)
		: mShader(data)
	{}

	void Shade(const Ray& r, rgba* result) const {
		Ray ray_mod = r;
		Modify(ray_mod);
		mShader->Shade(ray_mod, result);
	}
};
