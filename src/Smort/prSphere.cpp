#include "prSphere.h"

prSphere::prSphere(const vec3 center, real radius, const shBase* MatFront, const shBase* MatBack)
	: mCenter		(center)
	, mNorth		(0.0, -1.0, 0.0)
	, mEquador		(0.0, 0.0, 1.0)
	, mRadius		(radius)
	, mMaterialFront(MatFront)
	, mMaterialBack	(MatBack?MatBack:MatFront)
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
		if (i2 > r.minDist) {
			if (mMaterialFront && i1 < r.dist && i1 > r.minDist) {
				return true;
			} else if (mMaterialBack && i2 < r.dist) {
				return true;
			}
		}
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
		if (i2 > r.minDist) {
			if (mMaterialFront && i1 < r.dist && i1 > r.minDist) {
				//outside
				r.gState.object = this;
				r.dist = i1;
			} else if (mMaterialBack && i2 < r.dist) {
				//inside
				r.gState.object = this;
				r.dist = i2;
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
	(r.gState.inside ? mMaterialBack : mMaterialFront)->Shade(r, result);
}

// bool prSphere::TestPoint(vec3 &p) {
// 	return mCenter.sqr_distance(p) <= mSqrRadius;
// }

aabb
prSphere::GetAabb() const
{
	return aabb(mCenter-mRadius, mCenter+mRadius);
}
