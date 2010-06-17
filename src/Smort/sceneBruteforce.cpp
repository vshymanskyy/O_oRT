#include "sceneBruteforce.h"

sceneBruteForce::sceneBruteForce(envBase* env)
	: sceneBase(env)
{
}

bool sceneBruteForce::Hits(const Ray &r) const{
	for (List<prBase*>::Iterator i = mPrimitives.First(); i != mPrimitives.End(); ++i) {
		if (mPrimitives[i]->Hits(r)) {
			return true;
		}
	}
	return false;
}

void sceneBruteForce::Intersect(Ray &r) const{
	//if (mAabb.RayEntryExit(r.Position, r.Direction, NULL, NULL, NULL, NULL)) {
		real prevDistance = cMath::Huge;
		for (List<prBase*>::Iterator i = mPrimitives.First(); i != mPrimitives.End(); ++i) {
			mPrimitives[i]->Intersect(r);
			if (r.dist < prevDistance) {
				prevDistance = r.dist;
			}
		}
	//}
}

aabb sceneBruteForce::GetAabb() const{
	return mAabb;
}

void sceneBruteForce::Precompute() {
	/*List<prBase*>::Iterator i = mPrimitives.First();
	mAabb = mPrimitives[i]->GetAabb();
	++i;
	for (; i != mPrimitives.End(); ++i) {
		mAabb.EnlargeToEnclose(mPrimitives[i]->GetAabb());
	}*/
}

void sceneBruteForce::Add(prBase* p) {
	mPrimitives.Append(p);
}

void sceneBruteForce::Remove(prBase* p) {

}
