#include "slFileRa2.h"

bool slFileRa2::Load()
{
	File file(mFileName, true);
	if(SetGeometry){
		//read geometry
		while(!file.EoF()){
			vec3 a(file.ReadFloat(),file.ReadFloat(),file.ReadFloat());
			vec3 b(file.ReadFloat(),file.ReadFloat(),file.ReadFloat());
			vec3 c(file.ReadFloat(),file.ReadFloat(),file.ReadFloat());
			prTriangle* t = new prTriangle(
				a,b,c,
				vec2(0.0,0.0),
				vec2(0.0,1.0),
				vec2(1.0,1.0),
				mDefaultMat
				);
			mScene->Primitives()->Add(t);
		}
	}
	if(SetCamera){
		//generate camera description file name
		char* path = file.GetPath();
		char* name = file.GetName();
		char* f_cam = new char[1024];
		strcpy_s(f_cam, 1024, path);
		strcat_s(f_cam, 1024, name);
		strcat_s(f_cam, 1024, ".txt");
		delete path;
		delete name;
		//read camera data
		File cam_file(f_cam);
		vec3 pos(cam_file.ReadFloat(),cam_file.ReadFloat(),cam_file.ReadFloat());
		vec3 look_at(cam_file.ReadFloat(),cam_file.ReadFloat(),cam_file.ReadFloat());
		vec3 dir = (look_at-pos).normalize();

		vec3 up;
		switch(cam_file.ReadInt()){
			case 0:
				up.SetUnitX();
				break;
			case 1:
				up.SetUnitY();
				break;
			case 2:
				up.SetUnitZ();
				break;
		}
		//setup camera
		mScene->AddCamera(
			new camThinLens(pos, dir, up, 2.0, 1.0, 1.0)
		);
	}
	return true;
}