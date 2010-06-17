#include "envCubic.h"

envCubic::envCubic(shBase* up, shBase* down, shBase* right, shBase* left, shBase* front, shBase* back )
{
	mSides[cfUp]	= up;
	mSides[cfDown]	= down;
	mSides[cfRight]	= right;
	mSides[cfLeft]	= left;
	mSides[cfFront]	= front;
	mSides[cfBack]	= back;
}

void
envCubic::Shade(Ray &r, rgba* result) const
{
	if ((r.dir.x.Abs() >= r.dir.y.Abs()) && (r.dir.x.Abs() >= r.dir.z.Abs())) {
		if (r.dir.x > real(0)) {
			r.gState.textureUV.x = real(1) - (r.dir.z / r.dir.x+ real(1)) * real(0.5);
			r.gState.textureUV.y = real(1) - (r.dir.y / r.dir.x+ real(1)) * real(0.5);
			mSides[cfRight]->Shade(r, result);
		} else { //if (r.Direction.x < real(0)) {
			r.gState.textureUV.x = real(1) - (r.dir.z / r.dir.x + real(1)) * real(0.5);
			r.gState.textureUV.y =             (r.dir.y / r.dir.x + real(1)) * real(0.5);
			mSides[cfLeft]->Shade(r, result);
		}
	}else if ((r.dir.y.Abs() >= r.dir.x.Abs()) && (r.dir.y.Abs() >= r.dir.z.Abs())) {
		if (r.dir.y > real(0)) {
			r.gState.textureUV.x = 			(r.dir.x / r.dir.y + real(1)) * real(0.5);
			r.gState.textureUV.y = 			(r.dir.z / r.dir.y + real(1)) * real(0.5);
			mSides[cfUp]->Shade(r, result);
		} else { // if (r.Direction.y < real(0)) {
			r.gState.textureUV.x = real(1) - (r.dir.x / r.dir.y + real(1)) * real(0.5);
			r.gState.textureUV.y =             (r.dir.z / r.dir.y + real(1)) * real(0.5);
			mSides[cfDown]->Shade(r, result);
		}
	} else { // if ((.Abs(r.Direction.z) >= .Abs(r.Direction.x)) && (.Abs(r.Direction.z) >= .Abs(r.Direction.y))) {
		if (r.dir.z > real(0)) {
			r.gState.textureUV.x =             (r.dir.x / r.dir.z + real(1)) * real(0.5);
			r.gState.textureUV.y = real(1) - (r.dir.y / r.dir.z + real(1)) * real(0.5);
			mSides[cfFront]->Shade(r, result);
		} else { // if (r.Direction.z < real(0)) {
			r.gState.textureUV.x =             (r.dir.x / r.dir.z + real(1)) * real(0.5);
			r.gState.textureUV.y =             (r.dir.y / r.dir.z + real(1)) * real(0.5);
			mSides[cfBack]->Shade(r, result);
		}
	}
	r.dist = real(0.1);
	r.gState.normal = vec3(0.0, 0.0, 0.0);
	r.UpdatePoint();
}
