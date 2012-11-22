#include "shRaster.h"

#include <cmath>
#include <cstring>

rgba shRaster::GetPixel(int x,int y) const
{
	int index = bpp*(y*Width+x);
	index = cMath::ClampRange(index,0,bpp*Width*Height-1);
	switch (bpp) {
		case 1:
			return rgba((1.0f/255.0f)*data[index]);
		case 3:
		case 4:
			return rgba((1.0f/255.0f)*data[index],(1.0f/255.0f)*data[index+1],(1.0f/255.0f)*data[index+2]);
		case 12:
			index /= 4;
			float* f_data = (float*)data;
			return rgba(f_data[index],f_data[index+1],f_data[index+2]);
	}
	return rgba();
}

shRaster::shRaster(const char *fn, const FilteringMethod Filter)
	: filter(Filter)
{
	ILuint name;
	static bool ilInitialized = false;
	if (!ilInitialized) {
		ilInit();
		ilInitialized = true;
	}
	ilGenImages(1,&name);
	ilBindImage(name);

	const bool loaded = ilLoadImage(fn);
	assert(loaded);

	Width	= ilGetInteger(IL_IMAGE_WIDTH);
	Height	= ilGetInteger(IL_IMAGE_HEIGHT);
	bpp		= ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL),
	dataSize= ilGetInteger(IL_IMAGE_SIZE_OF_DATA),
	format	= ilGetInteger(IL_IMAGE_FORMAT),
	type	= ilGetInteger(IL_IMAGE_TYPE);
	data	= new ILubyte[dataSize];
	memcpy(data,ilGetData(),dataSize);
	ilDeleteImages(1,&name);

}

#define cubic(t)	((t)*(t)*(real(3.0)-real(2.0)*(t)))

void shRaster::Shade (const Ray &r, rgba* result) const
{
	const real
		x = r.gState.textureUV.x.Abs()*real(Width),
		y = r.gState.textureUV.y.Abs()*real(Height);
	switch(filter) {
		case (FM_NONE):{
			*result = GetPixel((int)(x)%Width,(int)(y)%Height);
		} case (FM_BILINEAR):{
			const real
				mx = x.Floor(),
				my = y.Floor(),
				x1 = mx.Mod(Width),
				y1 = my.Mod(Height);
			real
				x2 = x1 + real(1.0),
				y2 = y1 + real(1.0);
			{
				real tmp(Width-1);
				if (x2 > tmp)
					x2 = tmp;
				tmp= Height-1;
				if (y2 > tmp)
					y2 = tmp;
			}

			const real
				fx = x-mx,
				fy = y-my,
				ifx = real(1.0)-fx,
				ify = real(1.0)-fy;
			*result =
				GetPixel((int)(x1),(int)(y1))*(ifx*ify)+
				GetPixel((int)(x2),(int)(y1))*(fx*ify)+
				GetPixel((int)(x1),(int)(y2))*(ifx*fy)+
				GetPixel((int)(x2),(int)(y2))*(fx*fy);
			break;
		} case (FM_BICUBIC):{
			const real
				mx = x.Floor(),
				my = y.Floor(),
				x1 = mx.Mod(Width),
				y1 = my.Mod(Height);
			real
				x2 = x1 + real(1.0),
				y2 = y1 + real(1.0);

			{
				real tmp(Width-1);
				if (x2 > tmp)
					x2 = tmp;
				tmp= Height-1;
				if (y2 > tmp)
					y2 = tmp;
			}

			const real
				fx = cubic(x-mx),
				fy = cubic(y-my),
				ifx = real(1.0)-fx,
				ify = real(1.0)-fy;
			*result =
				GetPixel((int)(x1),(int)(y1))*(ifx*ify)+
				GetPixel((int)(x2),(int)(y1))*(fx*ify)+
				GetPixel((int)(x1),(int)(y2))*(ifx*fy)+
				GetPixel((int)(x2),(int)(y2))*(fx*fy);
			break;
		} default:{
			*result = GetPixel((int)(x)%Width,(int)(y)%Height);
			break;
		}
	}
}

shRaster::~shRaster() {
	delete[] data;
}
