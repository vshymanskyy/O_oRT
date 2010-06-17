#pragma once

#include "../defines.h"

#define tblTrigonometryCount		1024

namespace cMath{

	extern real* tCosSin;

	extern const real Tiny;
	extern const real Huge;
	extern const real PI;

	extern const real RadiansToDegrees;
	extern const real DegreesToRadians;

	extern const real invPI;
	extern const real invTwoPI;
	extern const real invLnTwo;

	void Init();

	template<class T>
	inline
	T ClampRange (const T& x, const T& min, const T& max) {
		if (x<min)
			return min;
		if (x>max)
			return max;
		return x;
	}

	template<class T>
	inline
	bool ClampMin (T *x, const T& min) {
		if ((*x)<min) {
			(*x) = min;
			return false;
		}
		return true;
	}

	template<class T>
	inline
	bool ClampMax (T *x, const T& max) {
		if ((*x)>max) {
			(*x) = max;
			return false;
		}
		return true;
	}

	template<class T>
	inline
	T& UpdateMin (const T& x, T& y) {
		if (x<y) {
			y = x;
		}
		return y;
	}

	template<class T>
	inline
	T& UpdateMax (const T& x, T& y) {
		if (x>y) {
			y = x;
		}
		return y;
	}

	template<class T>
	inline
	int UpdateMinMax (const T& x, T& min, T& max) {
		if (x>max) {
			max = x;
			return 1;
		}
		if (x<min) {
			min = x;
			return -1;
		}
		return 0;
	}

	template<class T>
	inline
	bool SameSignNonzero(const T& x, const T& y) {
		if (x<0) {
			return (y<0);
		} else if (0<x) {
			return (0<y);
		} else {
			return false;
		}
	}

	inline
	bool UpdateMaxAbs(real *maxabs, real updateval) {
		if (updateval > *maxabs) {
			*maxabs = updateval;
			return true;
		}else if (-updateval > *maxabs) {
			*maxabs = -updateval;
			return true;
		}else {
			return false;
		}
	}


/*
	inline
	double SqrtInv(double x) {
		const double xhalf = 0.5 * x;
		long long i = *(long long*)&x;
		i = 0x5fe6ec85e7de30daLL - (i>>1);
		x = *(double*)&i;
		x *= 1.5 - xhalf*x*x;
		// increases accuracy
		x *= 1.5 - xhalf*x*x;

		return x;
	}

	inline
	float SqrtInv(float x) {
		const float xhalf = 0.5f * x;
		int i = *(int*)&x;
		i = 0x5f3759df - (i>>1);
		x = *(float*)&i;
		x *= 1.5f - xhalf*x*x;
		// increases accuracy
		x *= 1.5f - xhalf*x*x;

		return x;
	}
*/
	inline
	real DegToRad(const real& n) {
		return n*DegreesToRadians;
	}

	inline
	real RadToDeg(const real& n) {
		return n*RadiansToDegrees;
	}
/*
#ifdef USE_TABLE_TRIGONOMETRY

#define half_tblTrigonometryCount	(tblTrigonometryCount/2)
#define quart_tblTrigonometryCount	(tblTrigonometryCount/4)
#define mask_tblTrigonometryCount	(tblTrigonometryCount-1)
#define	invPi_tblTrigonometryCount	(1.0/3.141592653589793238462643383)
	inline
	real Sin(real n) {
		const int i = n * (real)(half_tblTrigonometryCount * invPi_tblTrigonometryCount);
		if (i < 0)
			return tCosSin[(-((-i)&mask_tblTrigonometryCount)) + tblTrigonometryCount];
		else
			return tCosSin[i&mask_tblTrigonometryCount];
	}

	inline
	real Cos(real n) {
		const int i = n * (real)(half_tblTrigonometryCount * invPi_tblTrigonometryCount);
		if (i < 0)
			return tCosSin[((-i) + quart_tblTrigonometryCount)&mask_tblTrigonometryCount];
		else
			return tCosSin[(i + quart_tblTrigonometryCount)&mask_tblTrigonometryCount];
	}


#ifdef USE_APPROX_POW
	inline
	real Pow(const real& a, const int b) {
		return a/(real(b)-a*real(b)+a);
	}
#else
	inline
	real Pow(const real& a, const int b) {
		return pow((float)a, b);
	}
#endif
*/
}
