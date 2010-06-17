#include "sceneBase.h"

class  sceneBruteForce
	: public sceneBase
{

public:
	sceneBruteForce(envBase* env);

	void Intersect(Ray &r) const;
	bool Hits(const Ray &r) const;
	aabb GetAabb() const;

	void Precompute();

	void Add(prBase* p);
	void Remove(prBase* p);

private:
	List<prBase*> mPrimitives;
	aabb mAabb;
};
