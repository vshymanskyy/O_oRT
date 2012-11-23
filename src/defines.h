#pragma once

#ifdef WINDOWS
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cmath>
#include <omp.h>

#ifdef UNIX
	#include <unistd.h>
#else
	typedef __int8  int8_t;
	typedef __int16 int16_t;
	typedef __int32 int32_t;
	typedef __int64 int64_t;
	typedef unsigned __int8  uint8_t;
	typedef unsigned __int16 uint16_t;
	typedef unsigned __int32 uint32_t;
	typedef unsigned __int64 uint64_t;
#endif

using namespace std;

#undef assert
#define assert(x)
#undef countof
#define countof(arr) sizeof(arr)/sizeof(arr[0])

//#define USE_FIXED
#define USE_TABLE_TRIGONOMETRY
//#define USE_TABLE_RAND
//#define USE_APPROX_POW
//#define USE_FAST_SQRT

//#define SKEL_TRACE_ALL
//#define SKEL_TRACE_NORMAL

extern const int OMP_NR_THREADS;
extern int STAT_RAYS;
extern int NR_BOUNCES;
extern class skel* SKEL;

#ifdef USE_FIXED
	#include "fixed.h"
	typedef Fixed<int32_t, int64_t, 16> real;
#else
	#include "floating.h"
	typedef Floating<double> real;
#endif

typedef float channel;

template<class T>
inline
void Swap(T& v1, T& v2) {
	T t = v2;
	v2  = v1;
	v1  = t;
}

template<class T>
inline
T Min (const T& x, const T& y) {
	return (x<y) ? x : y;
}

template<class T>
inline
T Max (const T& x, const T& y) {
	return (y<x) ? x : y;
}

template<class T>
inline
T Min (const T& x, const T& y, const T& z) {
	T tmp = (x<y) ? x : y;
	if (z < tmp)
		tmp = z;
	return tmp;
}

template<class T>
inline
T Max (const T& x, const T& y, const T& z) {
	T tmp = (y<x) ? x : y;
	if (z > tmp)
		tmp = z;
	return tmp;
}

template <class T>
inline
T Average(const T& a, const T &b) {
	return (a + b) * T(0.5);
}

template <class T>
inline
T Lerp(const T& a, const T& b, const T& alpha) {
	return a * (T(1) - alpha) + b * alpha;
}

template<class T>
inline
T Square (const T& x) {
	return (x*x);
}

template<class T>
inline
T Cube (const T& x) {
	return (x*x*x);
}

inline
void SleepMs (unsigned t) {
#ifdef UNIX
	usleep(t*1000);
#else
	Sleep(t);
#endif
}
