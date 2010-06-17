#pragma once
#include "slFile.h"

class slFileRa2 : public slFile{
public:
	slFileRa2(Scene* scene, char* filename)
		:slFile(scene, filename)
	{}

	bool Load();
};

