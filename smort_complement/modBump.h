#pragma once
#include "shBase.h"

class modBump: public modBase{

private:
	const shBase* mData;

	void Modify(Ray &r) const{

	}

public:
	modBump(const shBase* shader, const shBase* data)
		: modBase(shader)
		, mData(data)
	{}

};
