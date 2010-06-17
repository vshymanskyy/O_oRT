#pragma once
#include "vec3.h"

class cRandom{
	static real Rand();
	static vec3 RandVec3();
public:
	static void Init();
	static void Seed(unsigned long seed);
	static real Random();
	static vec3 RandomVec3();

	template<class T> inline static T InRange(const T& a, const T& b) {
		return ((real)(a) + Random()*((real)(b)-(real)(a)));
	}
	
};

