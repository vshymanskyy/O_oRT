#pragma once
#include "..\O_oRT\Scene.h"
#include "..\StdShaderLib\StdShaderLib.h"

class slBase{
protected:
	Shader* mDefaultMat;
	Scene* mScene;

public:
	bool SetCamera:1;
	bool SetEnvironment:1;
	bool SetGeometry:1;
	bool SetLights:1;

	slBase(Scene* scene);

	void SetDefaultMat(Shader* mat);

	virtual bool Load() = NULL;
};
