#pragma once
#include "cMath.h"

struct vec4
{
	real x, y, z, w;

	vec4& Set(const real X, const real Y, const real Z, const real W) {
		x = X;
		y = Y;
		z = Z;
		w = W;
		return *this;
	}

	vec4() {}
	vec4(const vec4 &v):x(v.x),y(v.y),z(v.z),w(v.w) {}
	vec4(const vec4 *v):x(v->x),y(v->y),z(v->z),w(v->w) {}
	vec4(const real X, const real Y, const real Z, const real W):x(X),y(Y),z(Z),w(W) {}
	vec4(const real& val): x(val), y(val), z(val), w(val) {}

	real operator [](const unsigned int index) const{
		assert(index <= 3);
		switch(index) {
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
			default:
				return 0.0;
		}
	}
	vec4 operator -()const{return vec4(-x,-y,-z,-w);}

	bool operator ==(const vec4 &v)const{return (x==v.x)&&(y==v.y)&&(z==v.z)&&(w==v.w);}
	bool operator !=(const vec4 &v)const{return (x!=v.x)||(y!=v.y)||(z!=v.z)||(w!=v.w);}
	bool operator >(const vec4 &v)const{return (x>v.x)&&(y>v.y)&&(z>v.z)&&(w>v.w);}
	bool operator <(const vec4 &v)const{return (x<v.x)&&(y<v.y)&&(z<v.z)&&(w<v.w);}
	bool operator >=(const vec4 &v)const{return (x>=v.x)&&(y>=v.y)&&(z>=v.z)&&(w>=v.w);}
	bool operator <=(const vec4 &v)const{return (x<=v.x)&&(y<=v.y)&&(z<=v.z)&&(w<=v.w);}

	const vec4& operator += (const vec4& n)	{	*this = *this + n;  return *this;	}
	const vec4& operator -= (const vec4& n)	{	*this = *this - n;  return *this;	}
	const vec4& operator *= (const vec4& n)	{	*this = *this * n;  return *this;	}
	const vec4& operator /= (const vec4& n)	{	*this = *this / n;  return *this;	}
	const vec4& operator =  (const vec4& n) {	x = n.x; y = n.y; z = n.z; w = n.w;	return *this;	}

	vec4 operator +(const vec4 &v)const{return vec4(x+v.x, y+v.y, z+v.z, w+v.w);}
	vec4 operator -(const vec4 &v)const{return vec4(x-v.x, y-v.y, z-v.z, w-v.w);}
	vec4 operator *(const vec4 &v)const{return vec4(x*v.x, y*v.y, z*v.z, w*v.w);}
	vec4 operator /(const vec4 &v)const{return vec4(x/v.x, y/v.y, z/v.z, w/v.w);}

	vec4& SetZero()		{ x=real(0);	y=real(0);	z=real(0);  w=real(0);  return *this; }
	vec4& SetUnitX()	{ x=real(1);	y=real(0);	z=real(0);  w=real(0);  return *this; }
	vec4& SetUnitY()	{ x=real(0);	y=real(1);	z=real(0);  w=real(0);  return *this; }
	vec4& SetUnitZ()	{ x=real(0);	y=real(0);	z=real(1);   w=real(0);  return *this; }
	vec4& SetUnitW()	{ x=real(0);	y=real(0);	z=real(0);  w=real(1);   return *this; }
	vec4& SetNegUnitX() { x=-real(1);	y=real(0);	z=real(0);  w=real(0);  return *this; }
	vec4& SetNegUnitY() { x=real(0);	y=-real(1);   z=real(0);  w=real(0);  return *this; }
	vec4& SetNegUnitZ() { x=real(0);	y=real(0);	z=-real(1);  w=real(0);  return *this; }
	vec4& SetNegUnitW() { x=real(0);	y=real(0);	z=real(0);  w=-real(1);  return *this; }

	real sqr_length()const{
		return x*x+y*y+z*z+w*w;
	}
	real sqr_distance(const vec4 &v) const{
		const real
			a = x-v.x,
			b = y-v.y,
			c = z-v.z,
			d = z-v.z;
		return a*a+b*b+c*c+d*d;
	}
	real length() const {
		return sqr_length().Sqrt();
	}
	real distance(const vec4 &v) const {
		return sqr_distance(v).Sqrt();
	}
	real dot(const vec4 &v)const{
		return x*v.x+y*v.y+z*v.z+w*v.w;
	}
	vec4& invert() {
		x = -x;
		y = -y;
		z = -z;
		w = -w;
		return *this;
	}
	vec4& normalize() {
		real len = real(1) / length();
		x *= len;
		y *= len;
		z *= len;
		w *= len;
		return *this;
	}
	vec4 inverted()const{
		return vec4(-x, -y, -z, -w);
	}
	vec4 normalized()const{
		real len = real(1) / length();
		return vec4(x*len, y*len, z*len, w*len);
	}

};
