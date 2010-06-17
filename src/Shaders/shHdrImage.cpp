#include "shHdrImage.h"

shHdrImage::shHdrImage(int width, int height, rgba* data)
	: HdrImage	(width, height, data)
{
}

shHdrImage::shHdrImage(const char* fn)
	: HdrImage	(fn)
{
}

void shHdrImage::Shade(const Ray &r, rgba* result) const {
	const real x = r.gState.textureUV.x.Abs()*real(Width());
	const real y = r.gState.textureUV.y.Abs()*real(Height());
	*result = Buffer()[((int)(y)%Height())*Width()+((int)(x)%Width())];
}
