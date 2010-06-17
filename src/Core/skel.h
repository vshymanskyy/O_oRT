#pragma once

#include "rgba.h"
#include "vec3.h"
#include "List.h"
#include "File.h"

class skel
{

public:
	void addLine (const vec3& from, const vec3& to, const rgba& color);
	void save (const char* fn) const;
	unsigned getLineCount();

private:
	int addPoint(const vec3& p);

	struct line {
		int from;
		int to;
		rgba color;
	};

	List<vec3> mPoints;
	List<line> mLines;
};

inline
unsigned
skel::getLineCount() {
	return mLines.Count();
}

inline
int
skel::addPoint(const vec3& p) {
	int index = 0;
	for (List<vec3>::Iterator it = mPoints.First(); it != mPoints.End(); ++it, ++index) {
		if (mPoints[it] == p) {
			return index;
		}
	}
	mPoints.Append(p);
	return index;
}

inline
void
skel::addLine (const vec3& from, const vec3& to, const rgba& color) {
	#pragma omp critical
	{
	const int f = addPoint(from);
	const int t = addPoint(to);
	line l = {f, t, color};
	mLines.Append(l);
	}
}

inline
void
skel::save (const char* fn) const {
	FILE* f = fopen(fn, "w");
	fprintf(f, "SKEL\n");
	fprintf(f, "%d %d\n\n", mPoints.Count(), mLines.Count());

	for (List<vec3>::Iterator it = mPoints.First(); it != mPoints.End(); ++it) {
		fprintf(f, "%2.5f %2.5f %2.5f\n",
				(float)mPoints[it].x,
				(float)mPoints[it].y,
				(float)mPoints[it].z
				);
	}

	fprintf(f, "\n");

	for (List<line>::Iterator it = mLines.First(); it != mLines.End(); ++it) {
		fprintf(f, "2 %d %d %0.3f %0.3f %0.3f %0.3f\n",
				mLines[it].from,
				mLines[it].to,
				(float)mLines[it].color.r,
				(float)mLines[it].color.g,
				(float)mLines[it].color.b,
				(float)mLines[it].color.a
				);
	}
	fclose(f);
}
