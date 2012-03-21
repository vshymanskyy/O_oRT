#include "prTriangle.h"

#define ku mod3[k+1]
#define kv mod3[k+2]

const unsigned int mod3[] = { 0, 1, 2, 0, 1 };

prTriangle::prTriangle(	const vec3& A, const vec3& B, const vec3& C,
						const vec2& mA, const vec2& mB, const vec2& mC,
						const shBase* MatFront, const shBase* MatBack)
	: mPointA		(A)
	, mPointB		(B)
	, mPointC		(C)
	, mTexA			(mA)
	, mTexB			(mB)
	, mTexC			(mC)
	, mMaterialFront(MatFront)
	, mMaterialBack	(MatBack?MatBack:MatFront)
{
	edge1 = B - A;
	edge2 = C - A;
	edge1uv = mTexB - mTexA;
	edge2uv = mTexC - mTexA;

	//tangent
	//const real cp = edge1uv.y * edge2uv.x - edge1uv.x * edge2uv.y;
	//if (cp > cMath::Tiny) {
	//	const real mul = real(1) / cp;
	//	tangent   = ((edge1 * -edge2uv.y + edge2 * edge1uv.y) * mul).normalize();
	//	bitangent = ((edge1 * -edge2uv.x + edge2 * edge1uv.x) * mul).normalize();
	//}

	normal = edge1.cross(edge2);
	k = (normal.x.Abs() > normal.y.Abs()) ?
		((normal.x.Abs() > normal.z.Abs()) ? 0 : 2):
		((normal.y.Abs() > normal.z.Abs()) ? 1 : 2);
	const int u = ku;
	const int v = kv;
	real temp = real(1.0)/normal[k];
	nu = normal[u] * temp;
	nv = normal[v] * temp;
	nd = normal.dot(mPointA) * temp;
	temp = real(1.0) / (edge2[u] * edge1[v] - edge2[v] * edge1[u]);
	bnu =  edge2[u] * temp; //first line equation
	bnv = -edge2[v] * temp;
	cnu =  edge1[v] * temp; //second line equation
	cnv = -edge1[u] * temp;
	normal.normalize();
}

void prTriangle::Intersect(Ray &r) const {
	const real cosND = -normal.dot(r.dir);
	const bool inside = cosND < real(0.0);
	if (inside) {
		if (!mMaterialBack) {
			return;
		}
	} else {
		if (!mMaterialFront) {
			return;
		}
	}

	const real dist = (nd - r.pos[k] - nu * r.pos[ku] - nv * r.pos[kv]) / (r.dir[k] + nu * r.dir[ku] + nv * r.dir[kv]);
	if (dist < r.minDist || dist > r.dist) {
		return;
	}
	const real hu = r.pos[ku] + dist * r.dir[ku] - mPointA[ku];
	const real hv = r.pos[kv] + dist * r.dir[kv] - mPointA[kv];
	const real u = hv * bnu + hu * bnv;
	if (u < real(0.0)) {
		return;
	}
	const real v = hu * cnu + hv * cnv;
	if (v < real(0.0) || u + v > real(1.0)) {
		return;
	}

	r.gState.object = this;
	r.dist = dist;
	r.gState.textureUV.x = u;
	r.gState.textureUV.y = v;
	r.gState.inside = inside;
	//r.gState.normal = na + edge1 * r.gState + edge2 * r.gState;
	if (inside) {
		r.gState.normal = -normal;
		r.gState.cosND = -cosND;
	} else {
		r.gState.normal = normal;
		r.gState.cosND = cosND;
	}
}

bool
prTriangle::Hits(const Ray &r) const
{
	const real dist = (nd - r.pos[k] - nu * r.pos[ku] - nv * r.pos[kv]) / (r.dir[k] + nu * r.dir[ku] + nv * r.dir[kv]);
	if (dist < r.minDist || dist > r.dist) {
		return false;
	}
	const real hu = r.pos[ku] + dist * r.dir[ku] - mPointA[ku];
	const real hv = r.pos[kv] + dist * r.dir[kv] - mPointA[kv];
	const real u = hv * bnu + hu * bnv;
	if (u < real(0.0)) {
		return false;
	}
	const real v = hu * cnu + hv * cnv;
	if (v < real(0.0) || u + v > real(1.0)) {
		return false;
	}
	return true;
}

void
prTriangle::Shade(Ray &r, rgba* result) const
{
	r.gState.faceId = 0;
	r.UpdatePoint();

	//texture
	r.gState.textureUV = mTexA + edge1uv * r.gState.textureUV.x + edge2uv * r.gState.textureUV.y;

	//partials
	//r.gState.partialV = tangent;
	//r.gState.partialU = bitangent;

	//material
	(r.gState.inside ? mMaterialBack : mMaterialFront)->Shade(r, result);
}

// bool prTriangle::TestPoint(vec3 &p) {
// 	return ((p-mPointA).dot(normal)<=0);
// }

aabb prTriangle::GetAabb() const{
	vec3 a(Min(mPointA.x, mPointB.x, mPointC.x)
		,Min(mPointA.y, mPointB.y, mPointC.y)
		,Min(mPointA.z, mPointB.z, mPointC.z));
	vec3 b(Max(mPointA.x, mPointB.x, mPointC.x)
		,Max(mPointA.y, mPointB.y, mPointC.y)
		,Max(mPointA.z, mPointB.z, mPointC.z));
	return aabb(a, b);
}
