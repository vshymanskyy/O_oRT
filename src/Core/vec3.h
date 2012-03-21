#pragma once
#include "cMath.h"
#include "vec4.h"

struct vec3
{
	real x, y, z;

	vec3& Set(const real& X, const real& Y, const real& Z) {
		x = X;
		y = Y;
		z = Z;
		return *this;
	}

	vec3& SetFromHg(const vec4& v) {
		real wInv = real(1) / v.w;
		x = v.x * wInv;
		y = v.y * wInv;
		z = v.z * wInv;
		return *this;
	}


	vec3() {}
	vec3(const vec3& v):x(v.x),y(v.y),z(v.z) {}
	vec3(const vec3* v):x(v->x),y(v->y),z(v->z) {}
	vec3(const real& X,const real& Y,const real& Z):x(X),y(Y),z(Z) {}
	vec3(const real& val): x(val), y(val), z(val) {}
	vec3(const real val[3]): x(val[0]), y(val[1]), z(val[2]) {}

	real operator [](const unsigned int index) const{
		assert(index <=2);
		switch(index) {
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			default:
				return 0.0;
		}
	}
	vec3 operator -()const{return vec3(-x,-y,-z);}

	bool operator ==(const vec3 &v)const{return (x==v.x)&&(y==v.y)&&(z==v.z);}
	bool operator !=(const vec3 &v)const{return (x!=v.x)||(y!=v.y)||(z!=v.z);}
	bool operator >(const vec3 &v)const{return (x>v.x)&&(y>v.y)&&(z>v.z);}
	bool operator <(const vec3 &v)const{return (x<v.x)&&(y<v.y)&&(z<v.z);}
	bool operator >=(const vec3 &v)const{return (x>=v.x)&&(y>=v.y)&&(z>=v.z);}
	bool operator <=(const vec3 &v)const{return (x<=v.x)&&(y<=v.y)&&(z<=v.z);}

	const vec3& operator += (const vec3& n)	{	*this = *this + n;  return *this;	}
	const vec3& operator -= (const vec3& n)	{	*this = *this - n;  return *this;	}
	const vec3& operator *= (const vec3& n)	{	*this = *this * n;  return *this;	}
	const vec3& operator /= (const vec3& n)	{	*this = *this / n;  return *this;	}
	const vec3& operator =  (const vec3& n) {	x = n.x; y = n.y; z = n.z;	return *this;	}

	vec3 operator +(const vec3 &v)const{return vec3(x+v.x,y+v.y,z+v.z);}
	vec3 operator -(const vec3 &v)const{return vec3(x-v.x,y-v.y,z-v.z);}
	vec3 operator *(const vec3 &v)const{return vec3(x*v.x,y*v.y,z*v.z);}
	vec3 operator /(const vec3 &v)const{return vec3(x/v.x,y/v.y,z/v.z);}

	vec3& SetZero()		{ x=real(0);	y=real(0);	z=real(0);  return *this; }
	vec3& SetUnitX()	{ x=real(1);		y=real(0);	z=real(0);  return *this; }
	vec3& SetUnitY()	{ x=real(0);	y=real(1);	z=real(0);  return *this; }
	vec3& SetUnitZ()	{ x=real(0);	y=real(0);	z=real(1);   return *this; }
	vec3& SetNegUnitX() { x=-real(1);	y=real(0);	z=real(0);  return *this; }
	vec3& SetNegUnitY() { x=real(0);	y=-real(1);   z=real(0);  return *this; }
	vec3& SetNegUnitZ() { x=real(0);	y=real(0);	z=-real(1);  return *this; }

	static vec3 fromPolar(const real r,const real theta,const real phi) {
		return vec3(
			r*theta.Sin()*phi.Cos(),
			r*theta.Sin()*phi.Sin(),
			r*theta.Cos()
		);
	}
	real sqr_length() const {
		return x*x+y*y+z*z;
	}
	real sqr_distance(const vec3 &v) const {
		const real a = x-v.x;
		const real b = y-v.y;
		const real c = z-v.z;
		return a*a+b*b+c*c;
	}
	real length() const {
		return sqr_length().Sqrt();
	}
	real distance(const vec3 &v) const {
		return sqr_distance(v).Sqrt();
	}
	real dot(const vec3 &v) const {
		return x*v.x+y*v.y+z*v.z;
	}
	vec3 cross(const vec3 &v) const {
		return vec3(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x);
	}
	vec3& invert() {
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}
	vec3& normalize() {
		real len = real(1) / length();
		x *= len;
		y *= len;
		z *= len;
		return *this;
	}
	vec3 inverted()const{
		return vec3(-x, -y, -z);
	}
	vec3 normalized()const{
		real len = real(1) / length();
		return vec3(x*len, y*len, z*len);
	}

};
