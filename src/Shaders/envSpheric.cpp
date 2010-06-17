#include "envSpheric.h"

envSpheric::envSpheric(shBase* map)
	: mMap(map)
{
}

void
envSpheric::Shade(Ray &r, rgba* result) const
{
 	const real m = real(0.5) / (Square(r.dir.x) + Square(r.dir.y) + Square(r.dir.z + real(1.0))).Sqrt();

//	const real m = r.dir.z.ACos() / (Square(r.dir.x) + Square(r.dir.y).Sqrt() * real(2) * cMath::PI);
	r.gState.textureUV.x = real(0.5) + r.dir.x * m;
	r.gState.textureUV.y = real(0.5) - r.dir.y * m;
	r.dist = real(0.1);
	r.gState.normal = vec3(0.0, 0.0, 0.0);
	r.UpdatePoint();

	mMap->Shade(r, result);
}
