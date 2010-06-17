#pragma once
#include "vec3.h"

struct basis{

	vec3 u,v,w;

	inline basis() {}

	inline basis(const vec3 &U,const vec3 &V,const vec3 &W)
		: u(U)
		, v(V)
		, w(W)
	{
	}

	void flipU() {
		u.invert();
	}

	void flipV() {
		v.invert();
	}

	void flipW() {
		w.invert();
	}

	void swapUV() {
		vec3 t(u);
		u = v;
		v = t;
	}

	void swapVW() {
		vec3 t(v);
		v = w;
		w = t;
	}

	void swapWU() {
		vec3 t(w);
		w = u;
		u = t;
	}

	inline vec3 transform(const vec3 &a) {
		vec3 result;
		result.x = (a.x * u.x) + (a.y * v.x) + (a.z * w.x);
		result.y = (a.x * u.y) + (a.y * v.y) + (a.z * w.y);
		result.z = (a.x * u.z) + (a.y * v.z) + (a.z * w.z);
		return result;
	}

	inline vec3 untransform(const vec3 &a) {
		vec3 result;
		result.x = a.dot(u);
		result.y = a.dot(v);
		result.z = a.dot(w);
		return result;
	}

	inline basis untransform(const basis &b) {
		return basis(
			 untransform(b.u)
			,untransform(b.v)
			,untransform(b.w)
		);
	}

	inline basis transform(const basis &b) {
		return basis(
			 transform(b.u)
			,transform(b.v)
			,transform(b.w)
		);
	}

	inline static basis FromWV(const vec3& w,const vec3& v) {
		basis onb;
		onb.w = w.normalized();
		onb.u = v.cross(onb.w).normalize();
		onb.v = onb.w.cross(onb.u);
		return onb;
	}

	inline static basis FromW(const vec3& w) {
		basis onb;
		onb.w = w.normalized();
		if ((onb.w.x.Abs() < onb.w.y.Abs()) && (onb.w.x.Abs() < onb.w.z.Abs())) {
			onb.v.x = 0.0;
			onb.v.y = onb.w.z;
			onb.v.z = -onb.w.y;
		} else if (onb.w.y.Abs() < onb.w.z.Abs()) {
			onb.v.x = onb.w.z;
			onb.v.y = 0.0;
			onb.v.z = -onb.w.x;
		} else {
			onb.v.x = onb.w.y;
			onb.v.y = -onb.w.x;
			onb.v.z = 0.0;
		}
		onb.u = onb.v.normalize().cross(onb.w);
		return onb;
	}
};
