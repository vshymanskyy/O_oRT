#pragma once
#include "slBase.h"
#include "..\O_oRT\cRandom.h"

class slRandom : public slBase {
public:
	enum PrimType{
		Spheres,
		Triangles
	} mPrimType;

	int mCount;

	real mFloorRange;
	real mFloorHeight;

	real mRange;
	real mMin;
	real mMax;

	slRandom(Scene* scene, PrimType type)
		:slBase(scene)
		,mPrimType(type)
	{
		mCount = 7000;

		mFloorRange	= 50.0;
		mFloorHeight = -4.0;

		mRange	= 2.0;
		mMin	= 0.2;
		mMax	= 0.6;
	}

	bool Load(){
// 		if(SetCamera){
// 			mScene->AddCamera(
// 				new camThinLens(
// 					vec3(-15, 0, 0),
// 					vec3(1, 0, 0),
// 					vec3(0, 1, 0),
// 					4,1.0,1.0
// 				)
// 			);
// 		}

// 		if(SetEnvironment){
// 			mScene->SetEnvironment(
// 				new envCubeMap(
// 					 new dsRaster("Environments\\Cubic\\Mountains\\up.png")
// 					,new dsRaster("Environments\\Cubic\\Mountains\\down.png")
// 					,new dsRaster("Environments\\Cubic\\Mountains\\right.png")
// 					,new dsRaster("Environments\\Cubic\\Mountains\\left.png")
// 					,new dsRaster("Environments\\Cubic\\Mountains\\front.png")
// 					,new dsRaster("Environments\\Cubic\\Mountains\\back.png")
// 				)
// 			);
// 		}

		if(SetLights){
			mScene->Lights()->Add(new lsAmbient(rgba(0.2)));
			//mLights.Add(new lsPoint(rgba(10), vec3(3,-3,-3)));
			mScene->Lights()->Add(new lsDirectional(rgba(0.8),vec3(1.0,-1.0,-1.0)));
		}

		if(SetGeometry){
			//floor material
			Shader *mat = new shAddN(
				 new shCheckered(0.005, 0.005)
				,new mtMirror(new shConst(0.1))
			);

// 			ParamSet params;
// 			params.Set("PointA",vec3(-mFloorRange,mFloorHeight,-mFloorRange));
// 			params.Set("PointB",vec3(-mFloorRange,mFloorHeight, mFloorRange));
// 			params.Set("PointC",vec3(mFloorRange,mFloorHeight, mFloorRange));
// 			params.Set("TextureA",vec2(0,0));
// 			params.Set("TextureB",vec2(0,1));
// 			params.Set("TextureC",vec2(1,1));
// 			params.Set("Material",mat);
// 
// 			mScene->Primitives()->Add(
// 				Dynamic::CreateTriangle(params)
// 			);
// 
// 			params.Set("PointB",vec3(mFloorRange,mFloorHeight, mFloorRange));
// 			params.Set("PointC",vec3(mFloorRange,mFloorHeight,-mFloorRange));
// 			params.Set("TextureB",vec2(1,1));
// 			params.Set("TextureC",vec2(1,0));
// 
// 			mScene->Primitives()->Add(
// 				Dynamic::CreateTriangle(params)
// 			);

			for(int i=0; i<mCount; i++){
				real a = cRandom::InRange(-mRange,mRange);
				real b = cRandom::InRange(-mRange,mRange);
				real c = cRandom::InRange(-mRange,mRange);

// 				Shader* material = new shAddN(
// 					 new mtDiffuse(new shConst(rgba(cMath::Abs(a)/mRange * 2, cMath::Abs(b)/mRange * 2 , cMath::Abs(c)/mRange * 2)))
// 					,new mtMirror(new dsMarble(new dsTurbulence(new shPerlinImproved(),4,1.0,0.5)))
// 				);

				Shader* material = new mtGlass(
					new shConst(1.0)
					, new shAdd(new shConst(1.9), new shMul(new shPerlinImproved(), new shConst(0.8)))
					, new shConst(cMath::Abs(a)/mRange, cMath::Abs(b)/mRange, cMath::Abs(c)/mRange), new shConst(0.3));

				Shader* material2 = new mtDiffuse(new shConst(cMath::Abs(a)/mRange, cMath::Abs(b)/mRange, cMath::Abs(c)/mRange));

// 				params.Set("Material", material);
// 
// 				switch(mPrimType){
// 					case Spheres:
// 						params.Set("Center",vec3(a,b,c));
// 						params.Set("Radius",cRandom::InRange()(mMin,mMax));
// 						mScene->Primitives()->Add(Dynamic::CreateSphere(params));
// 						break;
// 					case Triangles:
// 						params.Set("PointA",vec3(a, b, c));
// 						params.Set("PointB",vec3(a+cRandom::InRange()(mMin,mMax),b+cRandom::InRange()(mMin,mMax),c+cRandom::InRange()(mMin,mMax)));
// 						params.Set("PointC",vec3(a+cRandom::InRange()(mMin,mMax),b+cRandom::InRange()(mMin,mMax),c+cRandom::InRange()(mMin,mMax)));
// 						params.Set("TextureA",vec2(0,0));
// 						params.Set("TextureB",vec2(0,1));
// 						params.Set("TextureC",vec2(1,1));
// 						mScene->Primitives()->Add(Dynamic::CreateTriangle(params));
// 						break;
// 				}

				switch(mPrimType){
					case Spheres:
						mScene->Primitives()->Add(
							new prSphere(
								vec3(a,b,c),
								cRandom::InRange(mMin,mMax),
								(cRandom::Random()>cMath::Half)?material: material2
							)
						);
						break;
					case Triangles:
						mScene->Primitives()->Add(
							new prTriangle(
								vec3(a, b, c),
								vec3(a+cRandom::InRange(mMin,mMax),b+cRandom::InRange(mMin,mMax),c+cRandom::InRange(mMin,mMax)),
								vec3(a+cRandom::InRange(mMin,mMax),b+cRandom::InRange(mMin,mMax),c+cRandom::InRange(mMin,mMax)),
								vec2(0.0,0.0),
								vec2(0.0,1.0),
								vec2(1.0,1.0),
								(cRandom::Random()>cMath::Half)?material: material2
							)
						);
						break;
				}
			}
		}

		return true;
	}

};

