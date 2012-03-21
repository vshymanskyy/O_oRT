#include "envSpheric.h"

envSpheric::envSpheric(shBase* map)
	: mMap(map)
{
}

void
envSpheric::Shade(Ray &r, rgba* result) const
{
	const real m = (real(0.5) / cMath::PI) * r.dir.z.ACos() / (Square(r.dir.x) + Square(r.dir.y)).Sqrt();


	r.gState.textureUV.x = real(0.5) + r.dir.x * m;
	r.gState.textureUV.y = real(0.5) + r.dir.y * m;

	mMap->Shade(r, result);
}

envSphericLanLon::envSphericLanLon(shBase* map)
	: mMap(map)
{
}

void
envSphericLanLon::Shade(Ray &r, rgba* result) const
{
	r.gState.textureUV.x = real(0.5) + real::ATan2(r.dir.x, -r.dir.z) / (cMath::PI * real(2));
	r.gState.textureUV.y = real(1.0) - r.dir.y.ACos() / cMath::PI;
	mMap->Shade(r, result);
}
