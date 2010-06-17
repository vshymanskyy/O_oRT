#pragma once

#include <assert.h>
#include "vec3.h"

class aabb {

public:
	aabb() {}

	aabb(const vec3& _min, const vec3& _max)
		: mMin(_min)
		, mMax(_max)
	{
		assert(WellFormed());
	}

	aabb(const aabb& b)
		: mMin(b.mMin)
		, mMax(b.mMax)
	{
	}

	void Set(const vec3& boxMin, const vec3& boxMax) {
		mMin = boxMin;
		mMax = boxMax;
		assert(WellFormed());
	}

	void SetNewAxisMin (int axisNum, real newMin) {
		switch (axisNum) {
			case 0: mMin.x = newMin; break;
			case 1: mMin.y = newMin; break;
			case 2:	mMin.z = newMin; break;
		}
	}

	void SetNewAxisMax (int axisNum, real newMax) {
		switch (axisNum) {
			case 0:	mMax.x = newMax; break;
			case 1:	mMax.y = newMax; break;
			case 2:	mMax.z = newMax; break;
		}
	}

	const vec3& GetMin() const { return mMin; }
	const vec3& GetMax() const { return mMax; }

	vec3& GetMin() { return mMin; }
	vec3& GetMax() { return mMax; }

	real GetMinX() const { return mMin.x; }
	real GetMaxX() const { return mMax.x; }
	real GetMinY() const { return mMin.y; }
	real GetMaxY() const { return mMax.y; }
	real GetMinZ() const { return mMin.z; }
	real GetMaxZ() const { return mMax.z; }

	bool IsFlatX() const { return mMin.x == mMax.x; }
	bool IsFlatY() const { return mMin.y == mMax.y; }
	bool IsFlatZ() const { return mMin.z == mMax.z; }

	bool WellFormed() const{
		return mMin.x <= mMax.x && mMin.y <= mMax.y && mMin.z<=mMax.z;
	}

	bool IsEmpty() const{
		return mMax.x == mMin.x || mMax.y == mMin.y || mMax.z == mMin.z;
	}

	void EnlargeToEnclose (const aabb& b) {
		cMath::UpdateMin(b.mMin.x, mMin.x);
		cMath::UpdateMin(b.mMin.y, mMin.y);
		cMath::UpdateMin(b.mMin.z, mMin.z);
		cMath::UpdateMax(b.mMax.x, mMax.x);
		cMath::UpdateMax(b.mMax.y, mMax.y);
		cMath::UpdateMax(b.mMax.z, mMax.z);
	}

	vec3 GetSize() const{
		return mMax - mMin;
	}

	real SurfaceArea() const{
		vec3 size = GetSize();
		return real(2)*(size.x*size.y + size.x*size.z + size.y*size.z);
	}

	void IntersectAgainst(const aabb& b) {
		cMath::UpdateMax(b.mMin.x, mMin.x);
		cMath::UpdateMax(b.mMin.y, mMin.y);
		cMath::UpdateMax(b.mMin.z, mMin.z);
		cMath::UpdateMin(b.mMax.x, mMax.x);
		cMath::UpdateMin(b.mMax.y, mMax.y);
		cMath::UpdateMin(b.mMax.z, mMax.z);
	}

	bool RayEntryExit(const vec3& startPos, const vec3& dir, real *entryDist, int *entryFaceId, real *exitDist, int *exitFaceId) const;
	bool RayEntryExit(const vec3& startPos, int signDirX, int signDirY, int signDirZ, const vec3& dirInv, real *entryDist, real *exitDist) const;

private:
	vec3 mMin;
	vec3 mMax;

};
