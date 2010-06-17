#pragma once

#include "shConst.h"
#include "shAdd.h"
#include "shSub.h"
#include "shMul.h"
#include "shDiv.h"

inline
shBase* operator +(const shBase* sh1, const rgba& v2) {
	return new shAdd(sh1, new shConst(v2));
}

inline
shBase* operator +(const rgba& sh1, const shBase* sh2) {
	return new shAdd(new shConst(sh1), sh2);
}

inline
shBase* operator -(const shBase* sh1, const rgba& v2) {
	return new shSub(sh1, new shConst(v2));
}

inline
shBase* operator -(const rgba& sh1, const shBase* sh2) {
	return new shSub(new shConst(sh1), sh2);
}

inline
shBase* operator *(const shBase* sh1, const rgba& v2) {
	return new shMul(sh1, new shConst(v2));
}

inline
shBase* operator *(const rgba& sh1, const shBase* sh2) {
	return new shMul(new shConst(sh1), sh2);
}

inline
shBase* operator /(const shBase* sh1, const rgba& v2) {
	return new shDiv(sh1, new shConst(v2));
}

inline
shBase* operator /(const rgba& sh1, const shBase* sh2) {
	return new shDiv(new shConst(sh1), sh2);
}
