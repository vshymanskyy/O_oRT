#pragma once
#include "cMath.h"

struct vec2
{

	real x, y;

	vec2& Set(const real X, const real Y) {
		x = X;
		y = Y;
		return *this;
	}

	vec2() {}
	vec2(const vec2& v):x(v.x),y(v.y) {}
	vec2(const real& X,const real& Y):x(X),y(Y) {}
	vec2(const real& val): x(val), y(val) {}

	real operator [](const unsigned int index)const{
		switch(index) {
			case 0:
				return x;
			case 1:
				return y;
		}
		return real(0);
	}

	vec2 operator -()const{return vec2(-x,-y);}

	bool operator ==(const vec2 &v)const{return (this->x==v.x)&&(this->y==v.y);}
	bool operator !=(const vec2 &v)const{return (this->x!=v.x)||(this->y!=v.y);}
	bool operator >(const vec2 &v)const{return (this->x>v.x)&&(this->y>v.y);}
	bool operator <(const vec2 &v)const{return (this->x<v.x)&&(this->y<v.y);}
	bool operator >=(const vec2 &v)const{return (this->x>=v.x)&&(this->y>=v.y);}
	bool operator <=(const vec2 &v)const{return (this->x>=v.x)&&(this->y>=v.y);}

	const vec2& operator += (const vec2& n)	{	*this = *this + n;  return *this;	}
	const vec2& operator -= (const vec2& n)	{	*this = *this - n;  return *this;	}
	const vec2& operator *= (const vec2& n)	{	*this = *this * n;  return *this;	}
	const vec2& operator /= (const vec2& n)	{	*this = *this / n;  return *this;	}
	const vec2& operator =  (const vec2 &n) {	x = n.x; y = n.y;	return *this;	}

	vec2 operator +(const vec2 &v)const{return vec2(this->x+v.x,this->y+v.y);}
	vec2 operator -(const vec2 &v)const{return vec2(this->x-v.x,this->y-v.y);}
	vec2 operator *(const vec2 &v)const{return vec2(this->x*v.x,this->y*v.y);}
	vec2 operator /(const vec2 &v)const{return vec2(this->x/v.x,this->y/v.y);}

	//static vec2 fromPolar(const real r,const real phi) {return vec3(r*sin(phi),r*cos(phi));}

	real sqr_length()const{return x*x+y*y;}
	real sqr_distance(const vec2 &v)const{real a=x-v.x;real b=y-v.y; return a*a+b*b;}
	real length()const{return (x*x+y*y).Sqrt();}
	real distance(const vec2 &v)const{real a=x-v.x;real b=y-v.y; return (a*a+b*b).Sqrt();}
	real dot(const vec2 &v)const{return x*v.x+y*v.y;}

	void invert() {x=-x; y=-y;}
	void normalize() { real len=real(1)/length(); x*=len; y*=len;}
	vec2 inverted()const{return vec2(-x,-y);}
	vec2 normalized()const{ real len=real(1)/length(); return vec2(x*len,y*len);}

};
