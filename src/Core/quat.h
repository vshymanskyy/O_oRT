#pragma once
#include "cMath.h"

#include "vec3.h"
struct quat {
	real w,x,y,z;

	quat() {}
	quat(const quat &q):w(q.w), x(q.x), y(q.y), z(q.z) {}
	quat(real W,const vec3 &V):w(W), x(V.x), y(V.y), z(V.z) {}
	quat(const real W,const real X,const real Y,const real Z):w(W),x(X),y(Y),z(Z) {}
	quat(const real Yaw, const real Pitch, const real Roll) {
		quat
			qy((Yaw/real(2)).Cos(),real(0),real(0),(Yaw/real(2)).Sin()),
			qp((Pitch/real(2)).Cos(),real(0),(Pitch/real(2)).Sin(),real(0)),
			qr((Roll/real(2)).Cos(),(Roll/real(2)).Sin(),real(0),real(0));
		*this=(qy*qp*qr);
	}

	real operator [](const unsigned int index)const{
		switch(index) {
			case 0:
				return w;
			case 1:
				return x;
			case 2:
				return y;
			case 3:
				return z;
		}
		return real(0);
	}

	quat operator +(const quat &q)const{
		return quat(w+q.w, x+q.x, y+q.y, z+q.z);
	}
	quat operator -(const quat &q)const{
		return quat(w-q.w, x-q.x, y-q.y, z-q.z);
	}
	quat operator *(const quat &q) {
		const real
			A=(w+x)*(q.w+q.x),
			B=(z-y)*(q.y-q.z),
			C=(x-w)*(q.y+q.z),
			D=(y+z)*(q.x-q.w),
			E=(x+z)*(q.x+q.y),
			F=(x-z)*(q.x-q.y),
			G=(w+y)*(q.w-q.z),
			H=(w-y)*(q.w+q.z);
		return quat(
			B+(-E-F+G+H)*real(0.5),
			A-(E+F+G+H)*real(0.5),
			-C+(E-F+G-H)*real(0.5),
			-D+(E-F-G+H)*real(0.5)
		);
	}

	real length()const{
		return (x*x+y*y+z*z+w*w).Sqrt();
	}
	quat conjugated() {
		return quat(w, -x, -y, -z);
	}
	quat normalize() {
		real len=real(1)/length(); x*=len; y*=len; z*=len; w*=len;return *this;
	}
	quat normalized()const{
		real len=real(1)/length(); return quat(w*len,x*len,y*len,z*len);
	}
	quat slerp(const quat &q,real t)const{
		real
			cosom = x*q.x+y*q.y+z*q.z+w*q.w,
			q1[4],omega,sinom,scale0,scale1;
		if (cosom<real(0)) {
			cosom = -cosom;
			q1[0] = -q.w;	q1[1] = -q.x;
			q1[2] = -q.y;	q1[3] = -q.z;
		} else {
			q1[0] = q.w;	q1[1] = q.x;
			q1[2] = q.y;	q1[3] = q.z;
		}
		if ((real(1)-cosom)>cMath::Tiny) {
			omega = cosom.ACos();
			sinom = omega.Sin();
			scale0 = ((real(1)-t)*omega).Sin()/sinom;
			scale1 = (t*omega).Sin()/sinom;
		} else {
			scale0 = real(1)-t;
			scale1 = t;
		}
		return quat(scale0*w+scale1*q1[0],scale0*x+scale1*q1[1],scale0*y+scale1*q1[2],scale0*z+scale1*q1[3]);
	}
	vec3 rotate(const vec3 &vec) {
		quat q = *this;
		q.normalize();
		const real tmp=this->w/real(2);
		q.x *= tmp.Sin();
		q.y *= tmp.Sin();
		q.z *= tmp.Sin();
		q.w = tmp.Cos();
		quat q2 = q*quat(0.0, vec.x, vec.y, vec.z)*q.conjugated();
		return vec3(q2.x, q2.y, q2.z);
	}

};
