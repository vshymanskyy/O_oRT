#pragma once

#include "ngBase.h"

class shWavelet: public ngBase{
private:
	int	 mSize;
	real *mNoise;

// 	real WProjectedNoise(const real p[3], real normal[3]) const;
// 	real WMultibandNoise(const real p[3], real s,real *normal,int firstBand,int nbands,real *w)const;

public:
	shWavelet(int size = 64, int seed = 0);
	~shWavelet(void);

	real Noise3D(const vec3& vec) const;
};
