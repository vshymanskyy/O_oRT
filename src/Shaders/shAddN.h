#pragma once
#include "shBase.h"
#include "List.h"

class shAddN: public shBase{

public:
	shAddN(
		const shBase* mt0,
		const shBase* mt1 = NULL,
		const shBase* mt2 = NULL,
		const shBase* mt3 = NULL,
		const shBase* mt4 = NULL,
		const shBase* mt5 = NULL,
		const shBase* mt6 = NULL,
		const shBase* mt7 = NULL,
		const shBase* mt8 = NULL,
		const shBase* mt9 = NULL
	);

	void Shade(const Ray &r, rgba* result) const;

private:
	List<const shBase*> mShaders;
};
