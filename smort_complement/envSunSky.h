#pragma once
#include "envBase.h"

class envSunSky : public envBase{

public:
	/**
		[in]  mSunDirection - the location of the sun.
		[in]  angle    - the number of degrees of arc occupied by the sun. 
		[in]  mTurbidity- is the ratio of the optical thickness of the haze and 
			  molecules to haze. Turbidity = (Tm + Th) / Tm. It is the fraction of 
			  scattering due to haze as opposed to molecules. 2-6 are good for clear
			  days
	*/
	envSunSky(const vec3& position, real turbidity);
	envSunSky(real latitude, real longitude, real time_of_day, int julian_day, real standard_meridian, real turbidity);

	void Set(const vec3& position, real turbidity);
	void Set(real latitude, real longitude, real time_of_day, int julian_day, real standard_meridian, real turbidity);

	rgba Shade(Ray &r) const;
	void Precompute();

private:
	real mTurbidity;			// turbidity of the atmosphere

	// precomputed:
	vec3 mSunDirection;			// normalized vector pointing to the sun
	real mSunTheta, mSunPhi;	// spherical coordinates of the sun mSunDirection
	real mSunCosTheta2;

	real perez_x[5], // coefficients for the perez functions
		 perez_y[5], 
		 perez_Y[5];
	real zenith_x,   // chromaticity at the zenith 
		 zenith_y,
		 zenith_Y;   // in cd/m^2

	real Perez(const real *lam, const real& theta, const real& gamma, const real& cosGamma, const real& lvz) const;
	static rgba toRGB(real x, real y, real Ys);
};
