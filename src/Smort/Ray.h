#pragma once

#include "vec2.h"
#include "vec3.h"
//#include "rgba.h"

class prBase;
class Scene;

struct Ray {
	vec3 pos, dir;
	const real minDist;
	real dist;
	const prBase* scene;
	const Ray* parent;

	struct gState_t {
		vec3 point;
		vec3 normal;
		vec3 partialV;
		vec3 partialU;
		vec2 textureUV;
		real cosND;
		const prBase *object;
		int faceId;
		bool inside;
	} gState;

	struct cState_t {
		int depth;
		real refrN;
	} cState;

	Ray(const prBase* Scene);
	Ray(const prBase* Scene, const vec3 &Pos, const vec3 &Dir, const int Depth, const Ray* Parent = NULL, const real Dist = cMath::Huge);

	void UpdatePoint();

	void UpdateNormal();
};

inline
Ray::Ray(const prBase* Scene)
	: minDist	(cMath::Tiny)
	, dist		(cMath::Huge)
	, scene		(Scene)
	, parent	(NULL)
{
	gState.object = NULL;
	cState.depth = NR_BOUNCES;
	cState.refrN = real(1.0);
}

inline
Ray::Ray(const prBase* Scene, const vec3 &Pos, const vec3 &Dir, const int Depth, const Ray* Parent, const real Dist)
	: pos		(Pos)
	, dir		(Dir)
	, minDist	(cMath::Tiny)
	, dist		(Dist)
	, scene		(Scene)
	, parent	(Parent)
{
	gState.object = NULL;
	cState.depth = Depth;
}

inline
void
Ray::UpdatePoint() {
	gState.point = pos + dir * dist;
}

inline
void
Ray::UpdateNormal() {
	gState.cosND = -gState.normal.dot(dir);
	gState.inside = gState.cosND < real(0.0);
	if (gState.inside) {
		gState.normal = -gState.normal;
		gState.cosND = -gState.cosND;
	}
}
