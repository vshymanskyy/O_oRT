#include "rgba.h"
#include "cMath.h"

bool rgba::WellFormed() {
	if ((r < 0.0f) || (g < 0.0f) || (b < 0.0f) || (a<0.0f) || (a>1.0f)) {
		return false;
	}
	return true;
}

rgba rgba::Abs() const{
	return rgba(fabs(r), fabs(g), fabs(b), a);
}

rgba rgba::Trim() const{
	return rgba(
		Min(Max(r, channel(0.0)), channel(1.0)),
		Min(Max(g, channel(0.0)), channel(1.0)),
		Min(Max(b, channel(0.0)), channel(1.0)),
		a
	);
}

float rgba::GetMax() const {
	return Max(r,g,b);
}

rgba rgba::Blended(const rgba &c, float alpha) const{
//  const float alpha_inv = 1-alpha;
// 	return rgba(
// 		r * alpha_inv + c.r * alpha,
// 		g * alpha_inv + c.g * alpha,
// 		b * alpha_inv + c.b * alpha
// 	);
	rgba result = c - *this;
	result *= alpha;
	result += *this;
	return result;
}
