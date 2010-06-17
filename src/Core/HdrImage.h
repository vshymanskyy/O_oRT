#pragma once

#include "List.h"
#include "rgba.h"

class HdrImage {

public:
	HdrImage(int width, int height, rgba* data = NULL);
	HdrImage(const char* fn);

	virtual ~HdrImage() {}

	bool Load(const char* fn);
	bool Save(const char* fn) const;

	void Clear();
	void FlipX();
	void FlipY();
	void FlipXY();

	int OffsetOf(const int x, const int y) const {
		return (y * Width()) + x;
	}

	int Width() const {
		return mWidth;
	}

	int Height() const {
		return mHeight;
	}

	rgba* Buffer() const {
		return mData;
	}

	void setSize(int w, int h);
	const char* getProperty(const char* name) const;

	void setProperty(const char* name, const char* value);
	void setProperty(const char* name, double value);
	void setProperty(const char* name, float value);
	void setProperty(const char* name, int value);

private:
	struct Property {
		char name[64];
		char* value;
	};

	rgba* mData;
	int mWidth;
	int mHeight;
	bool mFlipX;
	bool mFlipY;
	List<Property> mProperties;

	static const char* Format;
};
