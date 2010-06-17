#pragma once
#include "shBase.h"

class modScene
	: public modBase
{

private:
	prBase* mScene;

	void Modify(Ray &r) const {
		r.scene = mScene;
	}

public:
	modScene(const shBase* shader, prBase* data)
		: modBase(shader)
		, mScene(data)
	{}

};
