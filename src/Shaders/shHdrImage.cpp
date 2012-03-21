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
	//const int x = r.gState.textureUV.x.Abs()*real(Width());
	//const int y = r.gState.textureUV.y.Abs()*real(Height());
	//*result = Buffer()[(y%Height())*Width()+(x%Width())];
	//return;


	const real u = cMath::ClampRange(r.gState.textureUV.x.Abs() * real(Width()), real(0), real(Width()-2));
	const real v = cMath::ClampRange(r.gState.textureUV.y.Abs() * real(Height()), real(0), real(Height()-2));
	const unsigned umin = unsigned(u);
	const unsigned vmin = unsigned(v);
	const unsigned umax = umin+1;
	const unsigned vmax = vmin+1;
	const real ucoef = u - real(umin);
	const real vcoef = v - real(vmin);

	*result =	(Buffer()[umin + Width() * vmin] * (real(1) - ucoef)
			+	 Buffer()[umax + Width() * vmin] * ucoef) * (real(1) - vcoef)
			+	(Buffer()[umin + Width() * vmax] * (real(1) - ucoef)
			+	 Buffer()[umax + Width() * vmax] * ucoef) * vcoef;
}
