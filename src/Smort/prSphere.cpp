#include "prSphere.h"

prSphere::prSphere(const vec3 center, real radius, shBase *mat, bool frontFace, bool backFace)
	: mCenter		(center)
	, mNorth		(0.0, -1.0, 0.0)
	, mEquador		(0.0, 0.0, 1.0)
	, mRadius		(radius)
	, mMaterial		(mat)
	, mFrontFace	(frontFace)
	, mBackFace		(backFace)
	, mNcrossE		(mNorth.cross(mEquador))
	, mSqrRadius	(Square(mRadius))
{
}

bool
prSphere::Hits(const Ray &r) const
{
	const vec3 vec = r.pos-mCenter;
	const real b = -vec.dot(r.dir);
	real det = Square(b) - r.dir.dot(r.dir) * vec.sqr_length() + mSqrRadius;
	if (det > real(0)) {
		det = det.Sqrt();
		const real
			i1 = b - det,
			i2 = b + det;
		if (i2 > real(0)) {
			if (!mFrontFace || i1 < real(0)) {
				if (i2 > r.dist || i2 < r.minDist || !mBackFace) {
					return false;
				}
				return true;
			} else {
				if (i1 > r.dist || i1 < r.minDist) {
					return false;
				}
				return true;
			}
		}
		return false;
	}
	return false;
}

void
prSphere::Intersect(Ray &r) const
{
	const vec3 vec = r.pos-mCenter;
	const real b = -vec.dot(r.dir);
	real det = Square(b) - r.dir.dot(r.dir) * vec.sqr_length() + mSqrRadius;
	if (det > real(0)) {
		det = det.Sqrt();
		const real i1 = b - det;
		const real i2 = b + det;
		if (i2 > real(0)) {
			if (!mFrontFace || i1 < real(0.0)) {
				if (i2 > r.dist || i2 < r.minDist || !mBackFace) {
					return;
				}
				//inside
				r.gState.object = this;
				r.dist = i2;
			} else {
				if (i1 > r.dist || i1 < r.minDist) {
					return;
				}
				//outside
				r.gState.object = this;
				r.dist = i1;
			}
			return;
		}
	}
}

void
prSphere::Shade(Ray &r, rgba* result) const
{
	r.gState.faceId = 0;
	r.UpdatePoint();

	//normal
	r.gState.normal = (r.gState.point - mCenter) / mRadius;
	r.UpdateNormal();

	//texture
	r.gState.textureUV.y = (-r.gState.normal.dot(mNorth)).ACos();
	r.gState.textureUV.x = (r.gState.normal.dot(mEquador) / r.gState.textureUV.y.Sin()).ACos() * (real(1.0)/(cMath::PI*real(2.0)));
	//if (mNcrossE.dot(r.gState.normal) < real(0)) {
	//	r.gState.textureUV.x = real(1.0) - r.gState.textureUV.x;
	//}
	r.gState.textureUV.y /= cMath::PI;

	//partials
	//r.gState.partialU = mNorth;
	//r.gState.partialV = mNorth.cross(r.gState.normal);

	//material
	mMaterial->Shade(r, result);
}

// bool prSphere::TestPoint(vec3 &p) {
// 	return mCenter.sqr_distance(p) <= mSqrRadius;
// }

aabb
prSphere::GetAabb() const
{
	return aabb(mCenter-mRadius, mCenter+mRadius);
}
