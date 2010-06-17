#pragma once
#include "shBase.h"
#include "HdrImage.h"

class shHdrImage
	: public shBase
	, public HdrImage
{
public:
	shHdrImage(int width, int height, rgba* data = NULL);
	shHdrImage(const char* fn);
	~shHdrImage(void) {}

	void Shade (const Ray &r, rgba* result) const;
};
