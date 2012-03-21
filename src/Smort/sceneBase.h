#pragma once
#include "Ray.h"
#include "prBase.h"
#include "envBase.h"

class sceneBase
	: public prBase
{

private:
	const envBase* mEnvironment;

public:
	sceneBase(envBase* env)
		: mEnvironment(env)
	{
	}

	virtual void Add(prBase* p) = 0;
	virtual void Remove(prBase* p) = 0;

	void Shade(Ray &r, rgba* result) const {
		STAT_RAYS++;
		if (r.gState.object) {
			r.gState.object->Shade(r, result);
		} else if (mEnvironment) {
			mEnvironment->Shade(r, result);
		} else {
			*result = rgba(0.0);
		}
	}

};

class  sceneBruteForce
	: public sceneBase
{

public:
	sceneBruteForce(envBase* env)
		: sceneBase(env)
		, mPrimitivesQty(0)
	{
	}


	void Intersect(Ray &r) const {
		//if (mAabb.RayEntryExit(r.Position, r.Direction, NULL, NULL, NULL, NULL)) {
			real prevDistance = cMath::Huge;
			for (int i = 0; i != mPrimitivesQty; ++i) {
				mPrimitives[i]->Intersect(r);
				if (r.dist < prevDistance) {
					prevDistance = r.dist;
				}
			}
		//}
	}

	bool Hits(const Ray &r) const {
		for (int i = 0; i != mPrimitivesQty; ++i) {
			if (mPrimitives[i]->Hits(r)) {
				return true;
			}
		}
		return false;
	}

	aabb GetAabb() const {
		return mAabb;
	}

	void Precompute() {
		/*List<prBase*>::Iterator i = mPrimitives.First();
		mAabb = mPrimitives[i]->GetAabb();
		++i;
		for (; i != mPrimitives.End(); ++i) {
			mAabb.EnlargeToEnclose(mPrimitives[i]->GetAabb());
		}*/
	}

	void Add(prBase* p) {
		mPrimitives[mPrimitivesQty++] =p;
	}

	void Remove(prBase* p) {

	}

private:
	prBase* mPrimitives[64];
	int mPrimitivesQty;
	aabb mAabb;
};

