#include "slBase.h"

slBase::slBase(Scene* scene)
	:mScene(scene)
	,SetCamera(true)
	,SetEnvironment(true)
	,SetGeometry(true)
	,SetLights(true)
{
}

void slBase::SetDefaultMat(Shader* mat){
	mDefaultMat = mat;
}
