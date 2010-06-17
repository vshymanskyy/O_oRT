#pragma once
#include "slBase.h"
#include "..\O_oRT\cRandom.h"

class slTester : public slBase {

public:
	slTester(Scene* scene)
		:slBase(scene)
	{
	}

	bool Load(){
		if(SetCamera){
			camBase* cam = new camThinLens(
				vec3(-15.0, 0.0, 0.0),
				vec3(1.0, 0.0, 0.0),
				vec3(0.0, 1.0, 0.0),
				4.0,1.0,1.0
			);
			cam->PostProcessors()->Add(new ppBloom(0.02, 0.1));
			cam->PostProcessors()->Add(new ppTmReinhard(2.0, 2.0, 3.5));
			//cam->mPostProcessors.Add(new ppExposure(10.0));
			mScene->AddCamera(cam);
		}

		if(SetEnvironment){
			mScene->SetEnvironment(
				new envSpheric(new shRaster("Environments\\Spheric\\uffizi.hdr"))
			);
		}

		if(SetLights){
			mScene->Lights()->Add(new lsAmbient(rgba(0.2)));
			//mLights.Add(new lsPoint(rgba(10), vec3(3,-3,-3)));
			mScene->Lights()->Add(new lsDirectional(rgba(0.8),vec3(1.0,-1.0,-1.0)));
		}

		if(SetGeometry){
			Shader* material = //new shBlend(new dsPerlinImproved(), mScene->GetEnvironment(), new shConst(0.7)); 
			//new shAdd(
				new modGlossy(
					new mtMirror(new shConst(real(1.0)))
// 					new mtGlass(
// 						 new shConst(1.0)
// 						,new shConst(1.4)
// 						,new shConst(0.1, 0.6, 0.4)
// 						,new shConst(1.0)
// 					)
					,new shConst(real(0.3))
				);
				//,new mtDiffuse(new shConst(1.0), 1.0, 10)
			//);


		//new shMul(new shConst(1.0,0.0,0.0), new shCosND());

//		new mtMirror(new shConst(1.0));

			mScene->Primitives()->Add(
				new prSphere(
					vec3(0.0, 0.0, 0.0),
					0.7,
					material
				)
			);


			Shader* mat = //new mtSequence(
				//new mtDiffuse(new shConst(0.8)),
				//new mtDiffuse(new mtMirror(new shConst(1.0)));
				new shCheckered(0.1, 0.1);
			//);

			const real mFloorRange = 10.0;
			const real mFloorHeight = -2.0;

			ParamSet params;
			params.Set("PointA",vec3(-mFloorRange,mFloorHeight,-mFloorRange));
			params.Set("PointB",vec3(-mFloorRange,mFloorHeight, mFloorRange));
			params.Set("PointC",vec3(mFloorRange,mFloorHeight, mFloorRange));
			params.Set("TextureA",vec2(0.0,0.0));
			params.Set("TextureB",vec2(0.0,1.0));
			params.Set("TextureC",vec2(1.0,1.0));
			params.Set("Material",mat);

			mScene->Primitives()->Add(
				StdShaderLib::CreateTriangle(params)
			);

			params.Set("PointB",vec3(mFloorRange,mFloorHeight, mFloorRange));
			params.Set("PointC",vec3(mFloorRange,mFloorHeight,-mFloorRange));
			params.Set("TextureB",vec2(1.0,1.0));
			params.Set("TextureC",vec2(1.0,0.0));

			mScene->Primitives()->Add(
				StdShaderLib::CreateTriangle(params)
			);

		}

		return true;
	}

};

