#include "shMulN.h"

shMulN::shMulN(const shBase* mt0, const shBase* mt1 /*= NULL*/, const shBase* mt2 /*= NULL*/, const shBase* mt3 /*= NULL*/, const shBase* mt4 /*= NULL*/, const shBase* mt5 /*= NULL*/, const shBase* mt6 /*= NULL*/, const shBase* mt7 /*= NULL*/, const shBase* mt8 /*= NULL*/, const shBase* mt9 /*= NULL */) {
	if (mt0) mShaders.Append(mt0);
	if (mt1) mShaders.Append(mt1);
	if (mt2) mShaders.Append(mt2);
	if (mt3) mShaders.Append(mt3);
	if (mt4) mShaders.Append(mt4);
	if (mt5) mShaders.Append(mt5);
	if (mt6) mShaders.Append(mt6);
	if (mt7) mShaders.Append(mt7);
	if (mt8) mShaders.Append(mt8);
	if (mt9) mShaders.Append(mt9);
}

void
shMulN::Shade(const Ray &r, rgba* result) const
{
	*result = rgba(0);
	rgba r2;
	for (List<const shBase*>::Iterator i = mShaders.First(); i != mShaders.End(); ++i) {
		mShaders[i]->Shade(r, &r2);
		*result *= r2;
	}
}
