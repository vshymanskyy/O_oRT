#pragma once

template < typename base_t >
class Floating {

public:
	Floating() {}
	Floating(const Floating& num)		: mData(num.mData) {}
	Floating(const int& num)			: mData((base_t)num) {}
	Floating(const float& num)			: mData((base_t)num) {}
	Floating(const double& num)			: mData((base_t)num) {}
	Floating(const long double& num)	: mData((base_t)num) {}

	operator int()	const{	return (int)mData.value;	}
	operator float()const{	return (float)mData.value;	}
	operator bool()	const{	return fabs(mData.value) > base_t(0.0001);	}

	const Floating& operator=(const Floating& dn) {
		mData.value = dn.mData.value;
		return *this;
	}

	Floating operator+()const{	return Floating(data(mData.value));	}
	Floating operator-()const{	return Floating(data(-mData.value));	}
	bool operator!()const{	return abs(mData.value) < base_t(0.0001);	}

	Floating operator +  (const Floating& dn) const	{	return Floating(data(mData.value + dn.mData.value));	}
	Floating operator -  (const Floating& dn) const	{	return Floating(data(mData.value - dn.mData.value));	}
	Floating operator *  (const Floating& dn) const	{	return Floating(data(mData.value * dn.mData.value));	}
	Floating operator /  (const Floating& dn) const	{	return Floating(data(mData.value / dn.mData.value));	}

	const Floating& operator += (const Floating& n)	{	*this = *this + n; return *this;	}
	const Floating& operator -= (const Floating& n)	{	*this = *this - n; return *this;	}
	const Floating& operator *= (const Floating& n)	{	*this = *this * n; return *this;	}
	const Floating& operator /= (const Floating& n)	{	*this = *this / n; return *this;	}

	bool operator != (const Floating& dn) const	{	return abs(mData.value - dn.mData.value) > base_t(0.0001);	}
	bool operator == (const Floating& dn) const	{	return abs(mData.value - dn.mData.value) < base_t(0.0001);	}
	bool operator >  (const Floating& dn) const	{	return mData.value >  dn.mData.value;		}
	bool operator <  (const Floating& dn) const	{	return mData.value <  dn.mData.value;		}
	bool operator >= (const Floating& dn) const	{	return mData.value >= dn.mData.value;		}
	bool operator <= (const Floating& dn) const	{	return mData.value <= dn.mData.value;		}

	Floating Abs () const {
		return abs(mData.value);
	}

	Floating Floor () const {
		return floor(mData.value);
	}

	Floating Ceil () const {
		return ceil(mData.value);
	}

	Floating Sqrt() const {
		return sqrt(mData.value);
	}

	Floating Sin() const {
		return sin(mData.value);
	}

	Floating Cos() const {
		return cos(mData.value);
	}

	Floating ASin() const {
		return asin(mData.value);
	}

	Floating ATan() const {
		return asin(mData.value);
	}

	static Floating ATan2(const Floating& x, const Floating& y) {
		return atan2(x.mData.value, y.mData.value);
	}

	Floating ACos() const {
		return acos(mData.value);
	}

	Floating Pow(const Floating& n) const {
		return pow(mData.value, n.mData.value);
	}

	Floating Mod(const Floating& n) const {
		return fmod(mData.value, n.mData.value);
	}

	Floating Exp() const {
		return exp(mData.value);
	}

private:
	struct data {
		base_t value;
		data() {}
		data(const base_t& v) : value(v) {}
	} mData;

	Floating(const data& num)		: mData(num.value) {}

};
