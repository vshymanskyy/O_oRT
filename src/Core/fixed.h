#pragma once

template < typename base_t, typename ext_t, unsigned nseed = 10 >
class Fixed {

public:
	Fixed() {}
	Fixed(const Fixed& num)		: mData(num.mData) {}
	Fixed(const int& num)		: mData(num << NSEED) {}
	Fixed(const unsigned& num)	: mData(num << NSEED) {}
	Fixed(const float& num)		: mData(num  *  SEED) {}
	Fixed(const double& num)	: mData(num  *  SEED) {}
	Fixed(const long double& num)	: mData(num  *  SEED) {}

	operator int      () const				{	return mData.value >> NSEED;		}
	operator unsigned () const				{	return mData.value >> NSEED;		}
	operator float    () const				{	return float(mData.value)/SEED;		}
	operator double   () const				{	return double(mData.value)/SEED;	}
	operator bool     () const				{	return mData.value != 0;			}

	float ToFloat     () const				{	return float(mData.value)/SEED;		}

	Fixed operator << (unsigned n) const		{	return Fixed(data(mData.value << n));				}
	Fixed operator >> (unsigned n) const		{	return Fixed(data(mData.value >> n));				}
	Fixed operator +  (const Fixed& n) const	{	return Fixed(data(mData.value + n.mData.value));	}
	Fixed operator -  (const Fixed& n) const	{	return Fixed(data(mData.value - n.mData.value));	}
	Fixed operator *  (const Fixed& n) const	{	return Fixed(data((static_cast<ext_t>(mData.value) * n.mData.value) >> NSEED));	}
	Fixed operator /  (const Fixed& n) const	{	return Fixed(n.mData.value?data((static_cast<ext_t>(mData.value) << NSEED) / n.mData.value):0);	}
	Fixed operator +  (void)            const	{	return Fixed(data(mData.value));	}
	Fixed operator -  (void)            const	{	return Fixed(data(-mData.value));	}

	const Fixed& operator <<= (unsigned n)		{	*this = *this << n; return *this;	}
	const Fixed& operator >>= (unsigned n)		{	*this = *this >> n; return *this;	}
	const Fixed& operator += (const Fixed& n)	{	*this = *this + n;  return *this;	}
	const Fixed& operator -= (const Fixed& n)	{	*this = *this - n;  return *this;	}
	const Fixed& operator *= (const Fixed& n)	{	*this = *this * n;  return *this;	}
	const Fixed& operator /= (const Fixed& n)	{	*this = *this / n;  return *this;	}
	const Fixed& operator =  (const Fixed& n)	{	mData = n.mData;    return *this;	}

	bool operator != (const Fixed& n) const		{	return mData.value != n.mData.value;		}
	bool operator == (const Fixed& n) const		{	return mData.value == n.mData.value;		}
	bool operator >  (const Fixed& n) const		{	return mData.value >  n.mData.value;		}
	bool operator <  (const Fixed& n) const		{	return mData.value <  n.mData.value;		}
	bool operator >= (const Fixed& n) const		{	return mData.value >= n.mData.value;		}
	bool operator <= (const Fixed& n) const		{	return mData.value <= n.mData.value;		}
	bool operator !  (void)           const		{	return mData.value == 0;					}

	Fixed Abs () const {
		if (mData.value < 0) {
			return Fixed(data(~mData.value));
		} else {
			return *this;
		}
	}

	Fixed Floor () const {
		if (mData.value < 0) {
			return Fixed(data((mData.value >> NSEED) << NSEED));
		} else {
			return Fixed(data(((mData.value + 1) >> NSEED) << NSEED));
		}
	}

	Fixed Ceil () const {
		if (mData.value >= 0) {
			return Fixed(data((mData.value >> NSEED) << NSEED));
		} else {
			return Fixed(data(((mData.value + 1) >> NSEED) << NSEED));
		}
	}

	Fixed Sqrt() const {
		return sqrt(float(*this));
	}

	Fixed Sin() const {
		return sin(float(*this));
	}

	Fixed Cos() const {
		return cos(float(*this));
	}

	Fixed ASin() {
		return asin(float(*this));
	}

	Fixed ATan() const {
		return asin(mData.value);
	}

	static Fixed ATan2(const Fixed& x, const Fixed& y) {
		return atan2(float(x), float(y));
	}

	Fixed ACos() {
		return acos(float(*this));
	}

	Fixed Pow(const Fixed& n) {
		return pow(float(*this), float(n));
	}

	Fixed Mod(const Fixed& n) const {
		return mData.value % n.mData.value;
	}

	Fixed Exp() const {
		return exp(float(*this));
	}

private:
	enum { NSEED = nseed, SEED = 1<<NSEED };

	struct data {
		base_t value;
		data() {}
		data(const base_t& v) : value(v) {}
	} mData;

	Fixed(const data& num)		: mData(num.value) {}

};
