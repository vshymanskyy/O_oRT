#pragma once

#include "HdrImage.h"

class ppBase{
public:
	virtual ~ppBase() {};
	virtual void Process(HdrImage* image) const = 0;
};
