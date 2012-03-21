#pragma once

#include "aabb.h"
#include "shBase.h"
#include "skel.h"
#include "precomputable.h"

class prBase
	: public precomputable
{
public:
	virtual ~prBase() {};
	virtual void Intersect(Ray& r) const = 0;
	virtual void Shade(Ray& r, rgba* result) const = 0;
	virtual bool Hits(const Ray& r) const = 0;

	virtual aabb GetAabb() const = 0;

	void Trace(Ray& r, rgba* result) const {
		if (r.cState.depth <= 0) {
			*result = rgba(0.0);
			return;
		}
		Intersect(r);
		Shade(r, result);
		if (SKEL) {
			rgba rayCol(*result);
			rayCol.a = channel(0.3);
			SKEL->addLine(r.pos, r.gState.point, rayCol);
//#ifdef SKEL_TRACE_NORMAL
			rgba normalCol(r.gState.normal.x.Abs(), r.gState.normal.y.Abs(), r.gState.normal.z.Abs());
			SKEL->addLine(r.gState.point-r.gState.normal*real(0.1), r.gState.point+r.gState.normal*real(0.1), normalCol);
//#endif
		}
	}

};
