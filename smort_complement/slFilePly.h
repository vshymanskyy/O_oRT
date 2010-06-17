#pragma once
#include "slFile.h"

class slFilePly : public slFile{

public:
	slFilePly(Scene* scene, char* filename)
		:slFile(scene, filename)
	{}

	bool Load(){
		File file(mFileName);
		char* line;
		
		if(strcmp(line = file.ReadLine(),"ply")){
			delete line;
			return false;
		}
		delete line;

		while(!strcmp(line = file.ReadWord(), "end_header")){
			if(!strcmp(line,"format")){
				
			}else if(!strcmp(line,"format")){
			
			}
			delete line;
		}
		return true;
	}
};

