#pragma once
#include "Ray.h"
#include "prBase.h"
#include "envBase.h"
#include "List.h"

class sceneBase
	: public prBase
{

private:
	const envBase* mEnvironment;

public:
	sceneBase(envBase* env)
		: mEnvironment(env)
	{
	}

	virtual void Add(prBase* p) = 0;
	virtual void Remove(prBase* p) = 0;

	void Shade(Ray &r, rgba* result) const {
		STAT_RAYS++;
		if (r.cState.depth <= 0) {
			*result = rgba(0.0);
		} else if (r.gState.object) {
			r.gState.object->Shade(r, result);
		} else if (mEnvironment) {
			mEnvironment->Shade(r, result);
		} else {
			*result = rgba(0.0);
		}
	}

};
