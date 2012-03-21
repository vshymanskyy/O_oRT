#include "cRandom.h"

#include <math.h>
#define mtRand_N 624
#define M 397
#define MATRIX_A 0x9908b0df
#define UPPER_MASK 0x80000000
#define LOWER_MASK 0x7fffffff
#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000
#define TEMPERING_SHIFT_U(y) (y >> 11)
#define TEMPERING_SHIFT_S(y) (y << 7)
#define TEMPERING_SHIFT_T(y) (y << 15)
#define TEMPERING_SHIFT_L(y) (y >> 18)

unsigned long mt[mtRand_N];
int mti;

void cRandom::Seed(unsigned long seed) {
	mt[0]= seed & 0xffffffff;
	for (mti = 1; mti < mtRand_N; mti++)
		mt[mti] = (69069 * mt[mti - 1]) & 0xffffffff;
	unsigned long s = 373737;
	for (mti = 1; mti < mtRand_N; mti++) {
		mt[mti] ^= s;
		s = s * 5531 + 81547;
		s ^= (s >> 9) ^ (s << 19);
	}
}

real cRandom::Rand() {
	unsigned long y;
	static unsigned long mag01[2] = {0x0, MATRIX_A};
	if (mti >= mtRand_N) {
		int kk;
		for (kk=0;kk<mtRand_N-M;kk++) {
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		for (;kk<mtRand_N-1;kk++) {
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+(M-mtRand_N)] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		y = (mt[mtRand_N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
		mt[mtRand_N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1];
		mti = 0;
	}
	y = mt[mti++];
	y ^= TEMPERING_SHIFT_U(y);
	y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
	y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
	y ^= TEMPERING_SHIFT_L(y);
	return (real)((float)y * 2.3283064370807974e-10f);
}

vec3 cRandom::RandVec3() {
	vec3 dir;
	dir.z = real(2) * cRandom::Random() - real(1);
	const real t = real(2) * cMath::PI * cRandom::Random();
	const real w = (real(1) - Square(dir.z)).Sqrt();
	dir.x = w * t.Cos();
	dir.y = w * t.Sin();
	return dir;
}

#ifdef USE_TABLE_RAND

	#define tblRandCount		16384
	#define tblRandVec3Count	4096

	real* tRandom;
	vec3* tRandomVec3;

	void cRandom::Init() {
		Seed(123456789);

		tRandom = new real[tblRandCount];
		for (int i = 0; i<tblRandCount; i++)
			tRandom[i] = Rand();

		tRandomVec3 = new vec3[tblRandVec3Count];
		for (int i=0; i<tblRandVec3Count; i++)
			tRandomVec3[i] = RandVec3();
	}

	real cRandom::Random() {
		static int i = 0;
		i++;
		if (i>=tblRandCount)
			i=0;
		return tRandom[i];
	}

	vec3 cRandom::RandomVec3() {
		static int i = 0;
		i++;
		if (i>=tblRandVec3Count)
			i=0;
		return tRandomVec3[i];
	}

#else

	void cRandom::Init() {
		Seed(123456789);
	}

	real cRandom::Random() {
		return real(rand()) / real(RAND_MAX); //Rand();
	}

	vec3 cRandom::RandomVec3() {
		return RandVec3();
	}

#endif
