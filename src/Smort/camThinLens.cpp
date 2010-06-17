#include "camThinLens.h"
#include "cRandom.h"
#include "skel.h"

camThinLens::camThinLens(const vec3 &Pos, const vec3 &Dir, const vec3 &Up, const real D, real vW, real vH)
	: mPosition(Pos)
	, mDirection(Dir)
	, mUp(Up)
	, mViewSize(vW, vH)
	, mDistance(D)
	, mDofDistance(3.0)
	, mDofAperture(0.03)
	, mUseDoF (false)
{
	UpdateBasis();
}

void camThinLens::UpdateBasis() {
	mBase = basis::FromWV(mDirection, mUp);
}

void camThinLens::SaveSkel() {
	skel cam;
	cam.addLine(mPosition, mPosition+dr, rgba(0.5));
	cam.addLine(mPosition, mPosition+ur, rgba(0.5));
	cam.addLine(mPosition, mPosition+ul, rgba(0.5));
	cam.addLine(mPosition, mPosition+dl, rgba(0.5));

	cam.addLine(mPosition+dl, mPosition+dr, rgba(1.0));
	cam.addLine(mPosition+dr, mPosition+ur, rgba(1.0));
	cam.addLine(mPosition+ur, mPosition+ul, rgba(1.0));
	cam.addLine(mPosition+ul, mPosition+dl, rgba(1.0));

	cam.addLine(mPosition+dl+dx, mPosition+dl+dx+dy, rgba(1.0));
	cam.addLine(mPosition+dl+dx+dy, mPosition+dl+dy, rgba(1.0));
	cam.save("camera.skel");
}

void camThinLens::getRay(real x, real y, Ray* result) const {
	vec3 Pos = mPosition;
	vec3 Dir = dl + dx * x + dy * y;
	if (mUseDoF && (mDofAperture > cMath::Tiny)) {
		const vec3 temp_point = Pos + Dir * mDofDistance;
		Pos += unitX * cRandom::InRange(-half_aperture, half_aperture);
		Pos += unitY * cRandom::InRange(-half_aperture, half_aperture);
		Dir = temp_point - Pos;
	}
	Dir.normalize();
	result->pos = Pos;
	result->dir = Dir;
}

void camThinLens::Precompute() {
	half_aperture = mDofAperture/real(2);

	dl = mBase.transform(vec3(-mViewSize.x, -mViewSize.y, mDistance));
	dr = mBase.transform(vec3( mViewSize.x, -mViewSize.y, mDistance));
	ul = mBase.transform(vec3(-mViewSize.x,  mViewSize.y, mDistance));
	ur = mBase.transform(vec3( mViewSize.x,  mViewSize.y, mDistance));
	dx = (ur-ul)/real(mFrameWidth);
	dy = (ur-dr)/real(mFrameHeight);
	unitX = (ur-ul).normalized();
	unitY = (ur-dr).normalized();
}

