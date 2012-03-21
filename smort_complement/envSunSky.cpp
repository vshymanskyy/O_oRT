#include "envSunSky.h"
#include "envSunSky_constants.h"
#include "cMath.h"

envSunSky::envSunSky(const vec3& position, real turbidity){
	Set(position, turbidity);
	Precompute();
}

envSunSky::envSunSky(real latitude, real longitude, real time_of_day, int julian_day, real standard_meridian, real turbidity){
	Set(latitude, longitude, time_of_day, julian_day, standard_meridian, turbidity);
	Precompute();
}

void envSunSky::Set(const vec3& position, real turbidity){
	mSunDirection = position;
	mTurbidity = turbidity;

	mSunDirection.normalize();
	mSunPhi = atan2(mSunDirection.x, mSunDirection.z);
	mSunTheta = acos(mSunDirection.y);
}

void envSunSky::Set(real latitude, real longitude, real time_of_day, int julian_day, real standard_meridian, real turbidity){
	const real solar_time = time_of_day
		+ (0.170 * cMath::Sin(4.0 * cMath::PI * (julian_day - 80.0) / 373.0)
		- 0.129 * cMath::Sin(2.0 * cMath::PI * (julian_day - 8.0) / 355.0))
		+ 12.0 * (cMath::DegToRad(standard_meridian) - cMath::DegToRad(longitude)) / cMath::PI; 

	const real solar_declination = (0.4093 * cMath::Sin(2.0 * cMath::PI * (julian_day - 81.0) / 368.0));

	const real solar_altitude = cMath::ASin(cMath::Sin(cMath::DegToRad(latitude)) * cMath::Sin(solar_declination) -
		cMath::Cos(cMath::DegToRad(latitude)) * cMath::Cos(solar_declination) * cMath::Cos(cMath::PI * solar_time / 12.0));


	const real opp = -cMath::Cos(solar_declination) * cMath::Sin(cMath::PI * solar_time / 12.0);
	const real adj = -(cMath::Cos(cMath::DegToRad(latitude)) * cMath::Sin(solar_declination) +
		cMath::Sin(cMath::DegToRad(latitude)) * cMath::Cos(solar_declination) * cMath::Cos(cMath::PI * solar_time / 12.0));

	const real solar_azimuth = atan2(opp, adj);

	mTurbidity = turbidity;
	mSunPhi   = -solar_azimuth;
	mSunTheta = cMath::PI/2.0 - solar_altitude; 
	mSunDirection.x = cMath::Cos(mSunPhi) * cMath::Sin(mSunTheta);
	mSunDirection.z = cMath::Sin(mSunPhi) * cMath::Sin(mSunTheta);
	mSunDirection.y = cMath::Cos(mSunTheta);
}


void envSunSky::Precompute(){  
	mSunCosTheta2 = cMath::Cos(mSunTheta)*cMath::Cos(mSunTheta);

	const real theta2 = mSunTheta * mSunTheta;
	const real theta3 = theta2 * mSunTheta;
	const real T = mTurbidity;
	const real T2 = T * T;

	const real chi = (RealConst(4.0/9.0) - T / RealConst(120.0)) * (cMath::PI - cMath::Two * mSunTheta);
	
	zenith_Y = (RealConst(4.0453) * T - RealConst(4.9710)) * tan(chi) - RealConst(.2155) * T + RealConst(2.4192);
	zenith_Y *= RealConst(1000.0);  // conversion from kcd/m^2 to cd/m^2

	zenith_x =
		(RealConst(+0.00165)*theta3 - RealConst(0.00374)*theta2 + RealConst(0.00208)*mSunTheta + RealConst(0.00000)) * T2 +
		(RealConst(-0.02902)*theta3 + RealConst(0.06377)*theta2 - RealConst(0.03202)*mSunTheta + RealConst(0.00394)) * T +
		(RealConst(+0.11693)*theta3 - RealConst(0.21196)*theta2 + RealConst(0.06052)*mSunTheta + RealConst(0.25885));

	zenith_y =
		(RealConst(+0.00275)*theta3 - RealConst(0.00610)*theta2 + RealConst(0.00316)*mSunTheta  + RealConst(0.00000)) * T2 +
		(RealConst(-0.04214)*theta3 + RealConst(0.08970)*theta2 - RealConst(0.04153)*mSunTheta  + RealConst(0.00515)) * T +
		(RealConst(+0.15346)*theta3 - RealConst(0.26756)*theta2 + RealConst(0.06669)*mSunTheta  + RealConst(0.26688));

	perez_Y[0] = RealConst( 0.17872)*T - RealConst(1.46303);
	perez_Y[1] = RealConst(-0.35540)*T + RealConst(0.42749);
	perez_Y[2] = RealConst(-0.02266)*T + RealConst(5.32505);
	perez_Y[3] = RealConst( 0.12064)*T - RealConst(2.57705);
	perez_Y[4] = RealConst(-0.06696)*T + RealConst(0.37027);

	perez_x[0] = RealConst(-0.01925)*T - RealConst(0.25922);
	perez_x[1] = RealConst(-0.06651)*T + RealConst(0.00081);
	perez_x[2] = RealConst(-0.00041)*T + RealConst(0.21247);
	perez_x[3] = RealConst(-0.06409)*T - RealConst(0.89887);
	perez_x[4] = RealConst(-0.00325)*T + RealConst(0.04517);

	perez_y[0] = RealConst(-0.01669)*T - RealConst(0.26078);
	perez_y[1] = RealConst(-0.09495)*T + RealConst(0.00921);
	perez_y[2] = RealConst(-0.00792)*T + RealConst(0.21023);
	perez_y[3] = RealConst(-0.04405)*T - RealConst(1.65369);
	perez_y[4] = RealConst(-0.01092)*T + RealConst(0.05291);
}



real envSunSky::Perez(const real *lam, const real& theta, const real& gamma, const real& cosGamma2, const real& lvz) const{
	const real
		den = ((cMath::One+lam[0]*exp(lam[1]))*(cMath::One+lam[2]*exp(lam[3]*mSunTheta)+lam[4]*mSunCosTheta2)),
		num = ((cMath::One+lam[0]*exp(lam[1]/cMath::Cos(theta)))*(cMath::One+lam[2]*exp(lam[3]*gamma)+lam[4]*cosGamma2));
	return lvz*num/den;
}

rgba envSunSky::toRGB(real x, real y, real Ys){
	const real M1 = (RealConst(-1.3515) - RealConst(1.7703) *x + RealConst(5.9114) *y)/(RealConst(0.0241) + RealConst(0.2562)*x - RealConst(0.7341)*y);
	const real M2 = (RealConst(0.03)    - RealConst(31.4424)*x + RealConst(30.0717)*y)/(RealConst(0.0241) + RealConst(0.2562)*x - RealConst(0.7341)*y);

	real Yarea = 0.0;
	real X=0.0;
	real Y=0.0;
	real Z=0.0;

	for(int i=380, CIEidx=0, Sidx = (380-300)/10;	// match wavelengths - S starts at 300, CIE at 380
		i < 780;
		i+=10, CIEidx+=2, Sidx++)
	{
		const real amp = S0[Sidx] + M1 * S1[Sidx] + M2 * S2[Sidx];
		X += amp * CIE_X[CIEidx];
		Y += amp * CIE_Y[CIEidx];
		Z += amp * CIE_Z[CIEidx];
		Yarea += CIE_Y[CIEidx];
	}

	// scale luminance
	Ys /= 4000.0;
	X *= Ys / Y;
	Z *= Ys / Y;
	Y = Ys;

	// convert to RGB
	const real
		R = RealConst( 1.967) * X - RealConst(0.548) * Y - RealConst(0.297) * Z,
		G = RealConst(-0.955) * X + RealConst(1.938) * Y - RealConst(0.027) * Z,
		B = RealConst( 0.064) * X - RealConst(0.130) * Y + RealConst(0.982) * Z;

	return rgba(R, G, B);
}

rgba envSunSky::Shade(Ray &r) const{
	if(r.Direction.y >= 0.0){	// Above horizon
		const real
			cosGamma = r.Direction.dot(mSunDirection),	// cMath::Cos of the angle between direction and sun
			cosGamma2 = cosGamma*cosGamma,
			gamma = acos(cosGamma),
			theta = acos(r.Direction.y),
			x = Perez(perez_x, theta, gamma, cosGamma2, zenith_x),
			y = Perez(perez_y, theta, gamma, cosGamma2, zenith_y),
			Y = Perez(perez_Y, theta, gamma, cosGamma2, zenith_Y);
		return toRGB(x, y, Y);
	}else{
		const vec3 dir(r.Direction.x,-r.Direction.y,r.Direction.z);
		const real
			cosGamma = dir.dot(mSunDirection),
			cosGamma2 = cosGamma*cosGamma,
			gamma = acos(cosGamma),
			theta = acos(dir.y),
			x = Perez(perez_x, theta, gamma, cosGamma2, zenith_x),
			y = Perez(perez_y, theta, gamma, cosGamma2, zenith_y),
			Y = Perez(perez_Y, theta, gamma, cosGamma2, zenith_Y);

		return toRGB(x, y, cMath::Lerp(Y, cMath::Zero, cMath::ClampRange(dir.y*cMath::Three, cMath::Zero, cMath::One)));
	}
}
