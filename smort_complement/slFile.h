#pragma once
#include "slBase.h"
#include "..\O_oRT\File.h"

class slFile: public slBase{
protected:
	const char* mFileName;

public:

	slFile(Scene* scene, const char* filename)
		:slBase(scene)
		,mFileName(filename)
	{

	}
};
