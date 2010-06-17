#pragma once
#include "slBase.h"
#include "slRandom.h"

class slStrange : public slBase {

public:
	real mFloorRange;
	real mFloorHeight;

	slStrange(Scene* scene)
		:slBase(scene)
	{
		mFloorRange	= 50.0;
		mFloorHeight = 0.0;
	}

	bool Load(){
		if(SetCamera){
			mScene->AddCamera(
				new camThinLens(
					vec3(-15.0, 0.0, 0.0),
					vec3(1.0, 0.0, 0.0),
					vec3(0.0, 1.0, 0.0),
					4.0,1.0,1.0
				)
			);

			mScene->AddCamera(
				new camThinLens(
					vec3(-15.0, 0.0, 0.0),
					vec3(1.0, 0.0, 0.0),
					vec3(0.0, 1.0, 0.0),
					4.0,1.0,1.0
				)
			);
		}

		if(SetEnvironment){
			mScene->SetEnvironment(
				new envConst(rgba(0.2, 0.0, 0.2))
			);
		}

		if(SetLights){
			mScene->Lights()->Add(new lsAmbient(rgba(0.2)));
			//mLights.Add(new lsPoint(rgba(10), vec3(3,-3,-3)));
			mScene->Lights()->Add(new lsDirectional(rgba(0.8),vec3(1.0,-1.0,-1.0)));
		}

		if(SetGeometry){
			//floor material
			Shader *mat = //new shAddN(
				new mtDiffuse(new shCheckered(0.005, 0.005));
			//,new mtMirror(new shConst(0.1))
			//);

			ParamSet params;
// 			params.Set("PointA",vec3(-mFloorRange,mFloorHeight,-mFloorRange));
// 			params.Set("PointB",vec3(-mFloorRange,mFloorHeight, mFloorRange));
// 			params.Set("PointC",vec3(mFloorRange,mFloorHeight, mFloorRange));
// 			params.Set("TextureA",vec2(0,0));
// 			params.Set("TextureB",vec2(0,1));
// 			params.Set("TextureC",vec2(1,1));
// 			params.Set("Material",mat);
// 
// 			mScene->Primitives()->Add(
// 				StdShaderLib::CreateTriangle(params)
// 			);
// 
// 			params.Set("PointB",vec3(mFloorRange,mFloorHeight, mFloorRange));
// 			params.Set("PointC",vec3(mFloorRange,mFloorHeight,-mFloorRange));
// 			params.Set("TextureB",vec2(1,1));
// 			params.Set("TextureC",vec2(1,0));
// 
// 			mScene->Primitives()->Add(
// 				StdShaderLib::CreateTriangle(params)
// 			);


			Scene* scene2 = new Scene(new accKdTree(), NULL);
			slRandom loader(scene2, slRandom::Spheres);
			loader.mCount = 10;
			loader.Load();
			scene2->Sort();

// 			scene2->SetEnvironment(
// 				new envCubeMap(
// 					new dsRaster("Environments\\StPeters\\up.hdr")
// 					,new dsRaster("Environments\\StPeters\\down.hdr")
// 					,new dsRaster("Environments\\StPeters\\right.hdr")
// 					,new dsRaster("Environments\\StPeters\\left.hdr")
// 					,new dsRaster("Environments\\StPeters\\front.hdr")
// 					,new dsRaster("Environments\\StPeters\\back.hdr")
// 					,true
// 				)
// 			);

			Shader* material = new shAddN(
				 new mtDiffuse(new shConst(rgba(0.1, 0.1, 0.4)))
				,new modScene(new mtMirror(new shConst(0.8)), scene2)
			);

			params.Set("Material", material);
			params.Set("Center",vec3(0.0,1.0,0.0));
			params.Set("Radius",1.0);
			mScene->Primitives()->Add(StdShaderLib::CreateSphere(params));

		}

		return true;
	}

};

