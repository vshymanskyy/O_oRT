#pragma once
#include "shBase.h"

class ngBase
	: public shBase
{
private:
	int mDim;

public:
	ngBase(int dimensions)
		:mDim(dimensions)
	{}

	virtual ~ngBase() {
	}

	int GetDimensions() {
		return mDim;
	}

	virtual real Noise1D(const real& vec)const{
		return real(0);
	}

	virtual real Noise2D(const vec2& vec)const{
		return real(0);
	}

	virtual real Noise3D(const vec3& vec)const{
		return real(0);
	}

	void Shade(const Ray &r, rgba* result) const{
		switch(mDim) {
		case 1:
			*result = (channel)Noise1D(r.dist);
			break;
		case 2:
			*result = (channel)Noise2D(r.gState.textureUV);
			break;
		case 3:
			*result = (channel)Noise3D(r.gState.point);
			break;
		default:
			*result = rgba(0.0);
		}
	}
};
