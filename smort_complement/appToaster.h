#include "..\..\Dependencies\PixelToaster\include\PixelToaster.h"

#pragma comment (lib, "../../Dependencies/PixelToaster/lib/PixelToaster.lib")

#include "..\O_oRT\O_oRT.h"
#include "..\O_oRT\basis.h"
#include "..\O_oRT\quat.h"

using namespace PixelToaster;

class appToaster: public Listener{

private:
	Display mDisplay;
	bool* mKeyBuffer;
	vec2 mMousePos;

	Scene*	mScene;
	int		mCameraIndex;
	camThinLens* mCamera;

	void HandleKeyboard(){
		//mCamera
		if(mKeyBuffer[Key::W])
			mCamera->mPosition+=mCamera->mBase.w*0.5f;
		if(mKeyBuffer[Key::S])
			mCamera->mPosition-=mCamera->mBase.w*0.5f;
		if(mKeyBuffer[Key::A])
			mCamera->mPosition-=mCamera->mBase.u*0.5f;
		if(mKeyBuffer[Key::D])
			mCamera->mPosition+=mCamera->mBase.u*0.5f;
		if(mKeyBuffer[Key::R])
			mCamera->mPosition-=mCamera->mBase.v*0.5f;
		if(mKeyBuffer[Key::F])
			mCamera->mPosition+=mCamera->mBase.v*0.5f;
		//projection
		if(mKeyBuffer[Key::Q])
			mCamera->mDistance-=0.1;
		if(mKeyBuffer[Key::E])
			mCamera->mDistance+=0.1;
		//super sampling
		if(mKeyBuffer[Key::G])
			mCamera->Samples(mCamera->Samples()-1);
		if(mKeyBuffer[Key::H])
			mCamera->Samples(mCamera->Samples()+1);
		//DoF
		if(mKeyBuffer[Key::P])
			mCamera->mUseDoF = !mCamera->mUseDoF;
		if(mKeyBuffer[Key::OpenBracket])
			mCamera->mDofDistance -= 0.05;
		if(mKeyBuffer[Key::CloseBracket])
			mCamera->mDofDistance += 0.05;
		if(mKeyBuffer[Key::Separator])
			if(mCamera->mDofAperture > 0.01)
				mCamera->mDofAperture -= 0.01;
		if(mKeyBuffer[Key::Equals])
			mCamera->mDofAperture += 0.01;

		if(mKeyBuffer[Key::B])
			mCamera->PostProcess(!mCamera->PostProcess());

		if(mKeyBuffer[Key::One]){
			mCameraIndex++;
			if(mCameraIndex >= mScene->Cameras()->Size())
				mCameraIndex = 0;
			mCamera = (camThinLens*)mScene->Cameras()->GetItem(mCameraIndex);
		}
		if(mKeyBuffer[Key::Two]){
			mCameraIndex--;
			if(mCameraIndex < 0)
				mCameraIndex = mScene->Cameras()->Size()-1;
			mCamera = (camThinLens*)mScene->Cameras()->GetItem(mCameraIndex);
		}

	}

protected:
	void onKeyDown(DisplayInterface & display, Key key){
		mKeyBuffer[key] = true;
	}

	void onKeyUp(DisplayInterface & display, Key key){
		mKeyBuffer[key] = false;
	}

	void onKeyPressed(DisplayInterface & display, Key key){
		switch(key){
			case Key::F5:
				//mCamera->mFrame->Save("Shot.hdr");
				break;
		}
	}

	void onMouseMove(DisplayInterface & display, Mouse mouse){
		const vec2 delta(
			mouse.x-mMousePos.x
			,mouse.y-mMousePos.y
			);

		const real aspect = real(mCamera->mViewSize.height)/mCamera->mViewSize.width;
		vec3 angle;
		if(mouse.buttons.left){
			angle.x=delta.x*cMath::DegreesToRadians/mCamera->mDistance;
			angle.y=-delta.y*cMath::DegreesToRadians/mCamera->mDistance;

			quat rotationX(angle.x, mCamera->mBase.v);
			mCamera->mDirection=rotationX.rotate(mCamera->mBase.w).normalize();
			mCamera->UpdateBasis();

			quat rotationY(angle.y, mCamera->mBase.u);
			mCamera->mDirection=rotationY.rotate(mCamera->mBase.w).normalize();
			mCamera->UpdateBasis();

		}else if(mouse.buttons.right){
			angle.z=delta.y*cMath::DegreesToRadians;

			quat rotationZ(angle.z, mCamera->mBase.w);
			mCamera->mUp=rotationZ.rotate(mCamera->mBase.v).normalize();
			mCamera->UpdateBasis();
		}

		mMousePos.x = mouse.x;
		mMousePos.y = mouse.y;
	}

public:
	appToaster(Scene* scene){
		mScene = scene;
		mCameraIndex = 0;

		mKeyBuffer = new bool[256];
		for(int i=0; i<256; i++){
			mKeyBuffer[i] = false;
		}
	}

	~appToaster(){
		delete mKeyBuffer;
	}

	void Run(){
		mScene->Sort();

		mCamera = (camThinLens*)mScene->Cameras()->GetItem(mCameraIndex);

		mDisplay.open("O_oRT", mCamera->Width(), mCamera->Height());
		mDisplay.listener(this);

		vector<Pixel> pixels(mCamera->Width() * mCamera->Height());

		while (mDisplay.open()){
			HandleKeyboard();
			mCamera->Render();

			rgba* buff = mCamera->GetFrameBuffer()->mColor;
			for (int i = 0; i < mCamera->Height() * mCamera->Width(); ++i){
				pixels[i].r = buff[i].r;
				pixels[i].g = buff[i].g;
				pixels[i].b = buff[i].b;
			}

			mDisplay.update(pixels);
		}
	}

};