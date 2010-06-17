#include "aabb.h"
#include "cMath.h"

// Find intersection points with a ray.
bool aabb::RayEntryExit(const vec3& startPos, const vec3& dir, real *entryDist, int *entryFaceId, real *exitDist, int *exitFaceId) const{
 	vec3 dirInv(0.0, 0.0, 0.0);
/* 	int signDirX = cMath::Sign(dir.x);
 	int signDirY = cMath::Sign(dir.y);
 	int signDirZ = cMath::Sign(dir.z);

 	if (signDirX!=0) {
 		dirInv.x = real(1)/dir.x;
 	}
 	if (signDirY!=0) {
 		dirInv.y = real(1)/dir.y;
 	}
 	if (signDirZ!=0) {
 		dirInv.z = real(1)/dir.z;
 	}
 	*/
// 	return RayEntryExit(startPos, signDirX, signDirY, signDirZ, dirInv, entryDist, /*entryFaceId,*/ exitDist/*, exitFaceId*/);

	return true;
}

bool aabb::RayEntryExit(const vec3& startPos, int signDirX, int signDirY, int signDirZ, const vec3& dirInv, real *entryDist, real *exitDist) const{
	real& maxEnterDist = *entryDist;
	//int& maxEnterAxis = *entryFaceId;
	real& minExitDist = *exitDist;
	//int& minExitAxis = *exitFaceId;

	real mx, mn;
	if (signDirX!=0) {
		if (signDirX==1) {
			mx = mMax.x;
			mn = mMin.x;
		}
		else {
			mx = mMin.x;
			mn = mMax.x;
		}
		maxEnterDist = (mn-startPos.x)*dirInv.x;
		minExitDist = (mx-startPos.x)*dirInv.x;
	}
	else {
		if (startPos.x<mMin.x || startPos.x>mMax.x) {
			return false;
		}
		maxEnterDist = -cMath::Huge;
		minExitDist = cMath::Huge;
	}

	if (signDirY!=0) {
		if (signDirY==1) {
			mx = mMax.y;
			mn = mMin.y;
		}
		else {
			mx = mMin.y;
			mn = mMax.y;
		}
		real newEnterDist = (mn-startPos.y)*dirInv.y;
		real newExitDist = (mx-startPos.y)*dirInv.y;
		if (maxEnterDist<newEnterDist) {
			maxEnterDist = newEnterDist;
		}
		if (minExitDist>newExitDist) {
			minExitDist = newExitDist;
		}
	}
	else {
		if (startPos.y<mMin.y || startPos.y>mMax.y) {
			return false;
		}
	}

	if (signDirZ!=0) {
		if (signDirZ==1) {
			mx = mMax.z;
			mn = mMin.z;
		}
		else {
			mx = mMin.z;
			mn = mMax.z;
		}
		real newEnterDist = (mn-startPos.z)*dirInv.z;
		real newExitDist = (mx-startPos.z)*dirInv.z;
		if (maxEnterDist<newEnterDist) {
			maxEnterDist = newEnterDist;
		}
		if (minExitDist>newExitDist) {
			minExitDist = newExitDist;
		}
	}
	else {
		if (startPos.z<mMin.z || startPos.z>mMax.z) {
			return false;
		}
	}

	if (minExitDist<maxEnterDist) {
		return false;
	}
	return true;
}

