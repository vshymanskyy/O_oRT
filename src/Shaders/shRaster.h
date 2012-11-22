#pragma once
#include "shBase.h"
#include <IL/il.h>

class shRaster: public shBase{
private:
	ILubyte *data;
	int Width, Height;
	int bpp, dataSize, format, type;

	rgba GetPixel(int x,int y)const;
public:
	enum FilteringMethod{
		FM_NONE,
		FM_NEAREST,
		FM_LINEAR,
		FM_BILINEAR,
		FM_BICUBIC,
		//FM_BELL,
		//FM_BSPLINE,	//Cubic b-spline filtering
		//FM_CATMULLROM,//Catmull-Rom filtering
		//FM_TRILINEAR,
		//FM_ANISOTROPIC,
		FM_UNKNOWN
	} filter;

	shRaster(const char *fn,const FilteringMethod Filter = FM_BICUBIC);

	void Shade (const Ray &r, rgba* result) const;

	~shRaster();
};
