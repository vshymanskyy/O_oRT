#include "shWavelet.h"
#include "cRandom.h"

#define ARAD 16

static int Mod(int x, int n) {
	int m=x%n;
	return (m<0) ? m+n : m;
}

static void Downsample (real *from, real *to, int n, int stride) {
	const real *a;
	static const real aCoeffs[2*ARAD] = {
		0.000334,-0.001528, 0.000410, 0.003545,-0.000938,-0.008233, 0.002172, 0.019120,
		-0.005040,-0.044412, 0.011655, 0.103311,-0.025936,-0.243780, 0.033979, 0.655340,
		0.655340, 0.033979,-0.243780,-0.025936, 0.103311, 0.011655,-0.044412,-0.005040,
		0.019120, 0.002172,-0.008233,-0.000938, 0.003546, 0.000410,-0.001528, 0.000334
	};
	a = &aCoeffs[ARAD];
	for (int i=0; i<n/2; i++) {
		to[i*stride] = 0.0;
		for (int k=2*i-ARAD; k<=2*i+ARAD; k++)
			to[i*stride] += a[k-2*i] * from[Mod(k,n)*stride];
	}
}

static void Upsample(real *from, real *to, int n, int stride) {
	const real *p;
	static const real pCoeffs[4] = {
		0.25, 0.75, 0.75, 0.25
	};
	p = &pCoeffs[2];
	for (int i=0; i<n; i++) {
		to[i*stride] = 0.0;
		for (int k=i/2; k<=i/2+1; k++)
			to[i*stride] += p[i-2*k] * from[Mod(k,n/2)*stride];
	}
}

shWavelet::shWavelet(int size, int seed)
	: ngBase(3)
{
	if (size%2)
		size++; // tile size must be even

	mSize = size;

	cRandom::Seed(seed);

	int ix, iy, iz, i, sz=size*size*size;

	real *downsampled = new real[sz];
	real *upsampled = new real[sz];
	mNoise = new real[sz];

	// Step 1. Fill the tile with random numbers in the range -1 to 1.
	for (i=0; i<sz; i++) {
		mNoise[i] = cRandom::InRange(-real(1), real(1));
	}

	// Steps 2 and 3. Downsample and upsample the tile
	for (iy=0; iy<size; iy++) {
		for (iz=0; iz<size; iz++) { // each x row
			i = iy*size + iz*size*size;
			Downsample(&mNoise[i], &downsampled[i], size, 1);
			Upsample(&downsampled[i], &upsampled[i], size, 1);
		}
	}
	for (ix=0; ix<size; ix++) {
		for (iz=0; iz<size; iz++) { // each y row
			i = ix + iz*size*size;
			Downsample(&upsampled[i], &downsampled[i], size, size);
			Upsample(&downsampled[i], &upsampled[i], size, size);
		}
	}
	for (ix=0; ix<size; ix++) {
		for (iy=0; iy<size; iy++) { // each z row
			i = ix + iy*size;
			Downsample(&upsampled[i], &downsampled[i], size, size*size);
			Upsample(&downsampled[i], &upsampled[i], size, size*size);
		}
	}
	// Step 4. Subtract out the coarse-scale contribution
	for (i=0; i<sz; i++) {
		mNoise[i]-=upsampled[i];
	}
	// Avoid even/odd variance difference by adding odd-offset version of noise to itself.
	int offset=size/2;
	if (offset%2==0)
		offset++;
	for (i=0,ix=0; ix<size; ix++)
		for (iy=0; iy<size; iy++)
			for (iz=0; iz<size; iz++)
				downsampled[i++] = mNoise[ Mod(ix+offset,size) + Mod(iy+offset,size)*size + Mod(iz+offset,size)*size*size ];
	for (i=0; i<sz; i++) {
		mNoise[i]+=downsampled[i];
	}

	delete[] downsampled;
	delete[] upsampled;
}

shWavelet::~shWavelet() {
	delete[] mNoise;
}

real shWavelet::Noise3D(const vec3& vec) const{
	real items[3] = {vec.x, vec.y, vec.z};
	// Non-projected 3D noise
	int i, f[3], c[3], mid[3]; // f, c = filter, noise coeff indices
	real w[3][3], t, result =0.0;
	// Evaluate quadratic B-spline basis functions
	for (i=0; i<3; i++) {
		mid[i]=(items[i]-real(0.5)).Ceil();
		t=real(mid[i])-(items[i]-real(0.5));
		w[i][0]=t*t/real(2);

		w[i][1]=real(1)-w[i][0]-w[i][2];

		w[i][2]=(real(1)-t)*(real(1)-t)/real(2);
	}
	// Evaluate noise by weighting noise coefficients by basis function values
	for (f[2]=-1;f[2]<=1;f[2]++) {
		for (f[1]=-1;f[1]<=1;f[1]++) {
			for (f[0]=-1;f[0]<=1;f[0]++) {
				real weight=real(1);
				for (i=0; i<3; i++) {
					c[i]=Mod(mid[i]+f[i],mSize);
					weight*=w[i][f[i]+1];
				}
				result += weight * mNoise[c[2]*mSize*mSize+c[1]*mSize+c[0]];
			}
		}
	}
	return result;
}

//
// real shWavelet::WMultibandNoise(const real p[3],real s,real *normal,int firstBand,int nbands,real *w) const
// {
// 	real q[3], result=0, variance=0;
// 	int i, b;
// 	for (b=0; b<nbands && s+firstBand+b<0; b++) {
// 		for (i=0; i<=2; i++) {
// 			q[i]=2*p[i]*pow(real(2),firstBand+b);
// 		}
// 		result += (normal) ? w[b] * WProjectedNoise(q,normal) : w[b] * Noise3D(q);
// 	}
// 	for (b=0; b<nbands; b++) {
// 		variance+=w[b]*w[b];
// 	}
// 	// Adjust the noise so it has a variance of 1.
// 	if (variance)
// 		result /= cMath::Sqrt(variance * ((normal) ? 0.296 : 0.210));
// 	return result;
// }
//
// real shWavelet::WProjectedNoise(const real p[3], real normal[3]) const
// {
// 	// 3D noise projected onto 2D
// 	int i, c[3], min[3], max[3], n=mSize; // c = noise coeff location
// 	real support, result=0;
// 	// Bound the support of the basis functions for this projection direction
// 	for (i=0; i<3; i++) {
// 		support = 3*.Abs(normal[i]) + 3*cMath::Sqrt((1-normal[i]*normal[i])/2);
// 		min[i] = ceil(p[i] - (3*.Abs(normal[i]) + 3*cMath::Sqrt((1-normal[i]*normal[i])/2)));
// 		max[i] = floor(p[i] + (3*.Abs(normal[i]) + 3*cMath::Sqrt((1-normal[i]*normal[i])/2))); }
// 	// Loop over the noise coefficients within the bound.
// 	for (c[2]=min[2];c[2]<=max[2];c[2]++) {
// 		for (c[1]=min[1];c[1]<=max[1];c[1]++) {
// 			for (c[0]=min[0];c[0]<=max[0];c[0]++) {
// 				real t, t1, t2, t3, dot=0, weight=1;
// 				// Dot the normal with the vector from c to p
// 				for (i=0; i<3; i++) {dot+=normal[i]*(p[i]-c[i]);}
// 				// Evaluate the basis function at c moved halfway to p along the normal.
// 				for (i=0; i<3; i++) {
// 					t = (c[i]+normal[i]*dot/2)-(p[i]-1.5); t1=t-1; t2=2-t; t3=3-t;
// 					weight*=(t<=0||t>=3)? 0 : (t<1) ? t*t/2 : (t<2)? 1-(t1*t1+t2*t2)/2 : t3*t3/2;}
// 				// Evaluate noise by weighting noise coefficients by basis function values.
// 				result += weight * mNoise[Mod(c[2],n)*n*n+Mod(c[1],n)*n+Mod(c[0],n)];
// 			}
// 		}
// 	}
// 	return result;
// }
