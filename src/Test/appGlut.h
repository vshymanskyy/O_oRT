#include <GL/glut.h>

#include "basis.h"
#include "quat.h"
#include "skel.h"

#include "HdrImage.h"
#include "fsBase.h"
#include "prBase.h"
#include "camThinLens.h"

class appGlut {

private:
	bool mKeyBuffer[256];
	int mPixelation;
	int mWidth, mHeight;
	int	mPosX, mPosY;
	bool mNeedRender;

	camThinLens* mCamera;
	prBase* mGeometry;
	fsBase* mSampler;
	HdrImage* mFrame;

public:

	static appGlut* GetInstance() {
		static appGlut* inst = new appGlut();
		return inst;
	}

	void Run(int argc, char** argv, fsBase* sampler, camThinLens* cam, prBase* geom)
	{
		mPixelation	= 0;
		mWidth		= 512;
		mHeight		= 512;
		mCamera		= cam;
		mGeometry	= geom;
		mSampler	= sampler;
		mFrame		= new HdrImage(mWidth, mHeight);

		memset(mKeyBuffer, 0, 256*sizeof(bool));

		mSampler->setDisplayFunc(&Display);

		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
		glutInitWindowPosition(50, 50);
		glutInitWindowSize(mWidth, mHeight);
		glutCreateWindow("smort");
		glutIgnoreKeyRepeat(1);
		glutIdleFunc(&Idle);
		glutDisplayFunc(&Display);
		glutSpecialFunc(&KeyboardSpecDown);
		glutReshapeFunc(&Reshape);
		glutKeyboardFunc(&KeyboardDown);
		glutKeyboardUpFunc(&KeyboardUp);
		glutMotionFunc(&MouseMove);
		glutMouseFunc(&Mouse);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
		glClearAccum(0.0F, 0.0F, 0.0F, 0.0F);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_DITHER);
		glDisable(GL_FOG);
		glDisable(GL_LIGHTING);
		glDisable(GL_LOGIC_OP);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_TEXTURE_1D);
		glDisable(GL_TEXTURE_2D);
		glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
		glPixelTransferi(GL_RED_SCALE, GL_TRUE);
		glPixelTransferi(GL_RED_BIAS, GL_FALSE);
		glPixelTransferi(GL_GREEN_SCALE, GL_TRUE);
		glPixelTransferi(GL_GREEN_BIAS, GL_FALSE);
		glPixelTransferi(GL_BLUE_SCALE, GL_TRUE);
		glPixelTransferi(GL_BLUE_BIAS, GL_FALSE);
		glPixelTransferi(GL_ALPHA_SCALE, GL_TRUE);
		glPixelTransferi(GL_ALPHA_BIAS, GL_FALSE);

		glutMainLoop();
	}

	void Render() {
		mSampler->Render(mCamera, mGeometry, mFrame);
	}

	void setFrameSize(int w, int h){
		mFrame->setSize(w, h);
		mCamera->setFrameSize(w, h);
		mCamera->mViewSize.x = 1.0;
		mCamera->mViewSize.y = real(h)/real(w);
		mCamera->Precompute();
	}

	void Update(bool resize = true) {
		const int factor = 1 << mPixelation;
		if (resize) {
			setFrameSize(mWidth/factor, mHeight/factor);
		} else {
			glutReshapeWindow(mFrame->Width()*factor, mFrame->Height()*factor);
		}
		glPixelZoom (factor, factor);

		Render();

		glDrawPixels(mFrame->Width(), mFrame->Height(), GL_RGBA, GL_FLOAT, mFrame->Buffer());
		glutSwapBuffers();
		printf("%d x %d, %d spp: %1.3f, %1.2f Mrays\n", mFrame->Width(), mFrame->Height(), mSampler->getSamples(), 1.0/atof(mFrame->getProperty("RenderTime")), atof(mFrame->getProperty("RaysPerSec"))/1000000);

	}

	static void Display() {
		appGlut& app = *GetInstance();
		glPixelZoom (float(app.mWidth) / app.mFrame->Width(), float(app.mHeight) / app.mFrame->Height());
		glDrawPixels(app.mFrame->Width(), app.mFrame->Height(), GL_RGBA, GL_FLOAT, app.mFrame->Buffer());
		glFlush();
		glutSwapBuffers();
	}

	static void Reshape(int width, int height) {
		appGlut& app = *GetInstance();
		if (app.mWidth != width || app.mHeight != height) {
			app.mWidth = width;
			app.mHeight = height;

			char buff[32];
			sprintf(buff, "O_oRT [%d x %d * %d]", width, height, app.mSampler->getSamples());
			glutSetWindowTitle(buff);

			Display();
			app.mNeedRender = true;
		}
	}

	void HandleMouseFly(int deltaX, int deltaY) {
		const real rad = 0.017453292f;
		const real aspect = real(mCamera->mViewSize.y)/mCamera->mViewSize.x;
		vec3 angle;
		if (mKeyBuffer[0]) {
			angle.x=real(deltaX)*rad/mCamera->mDistance;
			angle.y=real(deltaY)*rad/mCamera->mDistance;

			quat rotationX(angle.x, mCamera->mBase.v);
			mCamera->mDirection=rotationX.rotate(mCamera->mBase.w).normalize();
			mCamera->UpdateBasis();

			quat rotationY(angle.y, mCamera->mBase.u);
			mCamera->mDirection=rotationY.rotate(mCamera->mBase.w).normalize();
			mCamera->UpdateBasis();
		}else if (mKeyBuffer[1]) {
		}else if (mKeyBuffer[2]) {
			angle.z=real(deltaY)*rad;

			quat rotationZ(angle.z, mCamera->mBase.w);
			mCamera->mUp=rotationZ.rotate(mCamera->mBase.v).normalize();
			mCamera->UpdateBasis();
		}
	}

	void HandleMouseRotate(int deltaX, int deltaY) {
		const real pi180 = 0.017453292f;

		static real X = 0, Y = 0;
		X += deltaY;
		Y -= deltaX;

		if (mKeyBuffer[0]) {
			real cam_distance = mCamera->mPosition.length();
			vec3 to(0,0,0);

			mCamera->mPosition.x = + (X*pi180).Cos()*(Y*pi180).Cos()*cam_distance;
			mCamera->mPosition.y = - (X*pi180).Sin()*(Y*pi180).Cos()*cam_distance;
			mCamera->mPosition.z = + (Y*pi180).Sin()*cam_distance;

			mCamera->mDirection = -mCamera->mPosition;
			mCamera->mDirection.normalize();

			mCamera->UpdateBasis();

			mCamera->mUp = mCamera->mBase.transform(vec3(0,1,0));
		}
	}

	static void MouseMove(int x, int y) {
		appGlut& app = *GetInstance();
		const int deltaX = x - app.mPosX;
		const int deltaY = y - app.mPosY;
		app.mPosX = x;
		app.mPosY = y;
		app.HandleMouseFly(deltaX, deltaY);
	}

	static void Mouse(int button, int state, int x, int y) {
		appGlut& app = *GetInstance();
		if (state==GLUT_UP) {
			app.mKeyBuffer[button]=false;
		} else {
			app.mKeyBuffer[button]=true;
			app.mPosX = x;
			app.mPosY = y;
		}
	}
	static void KeyboardDown(unsigned char key, int x, int y) {
		GetInstance()->mKeyBuffer[key] = true;

		if (key == 27) {
			exit(0);
		}
	}

	static void KeyboardUp(unsigned char key, int x, int y) {
		GetInstance()->mKeyBuffer[key] = false;
	}

	static void Idle() {
		GetInstance()->OnIdle();
	}

	void OnIdle()
	{
		if (mKeyBuffer[0] || mKeyBuffer[1] || mKeyBuffer[2]) {
			mNeedRender = true;
			//mPixelation = 2;
		} /*else if (mPixelation == 2) {
			mNeedRender = true;
			mPixelation = 0;
		}*/

		if (mKeyBuffer['w']) {
			mCamera->mPosition += mCamera->mBase.w*real(0.5); mNeedRender = true;
		}
		if (mKeyBuffer['s']) {
			mCamera->mPosition -= mCamera->mBase.w*real(0.5); mNeedRender = true;
		}
		if (mKeyBuffer['a']) {
			mCamera->mPosition -= mCamera->mBase.u*real(0.5); mNeedRender = true;
		}
		if (mKeyBuffer['d']) {
			mCamera->mPosition += mCamera->mBase.u*real(0.5); mNeedRender = true;
		}
		if (mKeyBuffer['r']) {
			mCamera->mPosition += mCamera->mBase.v*real(0.5); mNeedRender = true;
		}
		if (mKeyBuffer['f']) {
			mCamera->mPosition -= mCamera->mBase.v*real(0.5); mNeedRender = true;
		}
		//speed
		if (mKeyBuffer['W']) {
			mCamera->mPosition += mCamera->mBase.w*real(5.0); mNeedRender = true;
		}
		if (mKeyBuffer['S']) {
			mCamera->mPosition -= mCamera->mBase.w*real(5.0); mNeedRender = true;
		}
		if (mKeyBuffer['A']) {
			mCamera->mPosition -= mCamera->mBase.u*real(5.0); mNeedRender = true;
		}
		if (mKeyBuffer['D']) {
			mCamera->mPosition += mCamera->mBase.u*real(5.0); mNeedRender = true;
		}
		if (mKeyBuffer['R']) {
			mCamera->mPosition += mCamera->mBase.v*real(5.0); mNeedRender = true;
		}
		if (mKeyBuffer['F']) {
			mCamera->mPosition -= mCamera->mBase.v*real(5.0); mNeedRender = true;
		}
		//projection
		if (mKeyBuffer['q']) {
			mCamera->mDistance -= 0.1;
			mNeedRender = true;
		}
		if (mKeyBuffer['e']) {
			mCamera->mDistance += 0.1;
			mNeedRender = true;
		}
		//super sampling
		if (mKeyBuffer['g']) {
			if (mSampler->getSamples() > 1) {
				mSampler->setSamples(mSampler->getSamples()-1);
				mNeedRender = true;
				mKeyBuffer['g'] = false;
			}
		}
		if (mKeyBuffer['h']) {
			if (mSampler->getSamples() < 5) {
				mSampler->setSamples(mSampler->getSamples()+1);
				mNeedRender = true;
				mKeyBuffer['h'] = false;
			}
		}
		//DoF
		if (mKeyBuffer['p']) {
			mCamera->mUseDoF = !mCamera->mUseDoF;
			mKeyBuffer['p'] = false;
			mNeedRender = true;
		}
		if (mKeyBuffer['[']) {
			mCamera->mDofDistance -= 0.05;
			mNeedRender = true;
		}
		if (mKeyBuffer[']']) {
			mCamera->mDofDistance += 0.05;
			mNeedRender = true;
		}
		if (mKeyBuffer['-']) {
			if (mCamera->mDofAperture > real(0)) {
				mCamera->mDofAperture -= 0.01;
				mNeedRender = true;
			}
		}
		if (mKeyBuffer['=']) {
			mCamera->mDofAperture += 0.01;
			mNeedRender = true;
			usleep(50000);
		}
		if (mKeyBuffer['b']) {
			mSampler->setPostProcess(!mSampler->getPostProcess());
			mNeedRender = true;
			mKeyBuffer['b'] = false;
		}


		if (mKeyBuffer['n']) {
			if (mPixelation > 0) {
				mPixelation--;
				mNeedRender = true;
				mKeyBuffer['n'] = false;
			}
		}
		if (mKeyBuffer['m']) {
			if (mPixelation < 4) {
				mPixelation++;
				mNeedRender = true;
				mKeyBuffer['m'] = false;
			}
		}

		if (mKeyBuffer['t'] || mKeyBuffer['T']) {
			SKEL = new skel();
			const int factor = 1 << mPixelation;
			if (mKeyBuffer['t']) {
				//for (int i = 0; i < 512; i++) {
					Ray ray(mGeometry);
					rgba col;
					mCamera->getRay(mPosX/factor, mPosY/factor, &ray);
					mGeometry->Trace(ray, &col);
				//}
			} else if (mKeyBuffer['T']) {
				Render();
			}
			SKEL->save("trace.skel");
			printf("Trace saved (%d lines)\n", SKEL->getLineCount());
			delete SKEL;
			SKEL = NULL;
			mCamera->SaveSkel();
			mKeyBuffer['t'] = false;
			mKeyBuffer['T'] = false;
		}

		if (mKeyBuffer['.']) {
			if (NR_BOUNCES<20) {
				NR_BOUNCES ++;
				mNeedRender = true;
				mKeyBuffer['.'] = false;
			}
		}
		if (mKeyBuffer[',']) {
			if (NR_BOUNCES>1) {
				NR_BOUNCES --;
				mNeedRender = true;
				mKeyBuffer[','] = false;
			}
		}

		if (mKeyBuffer['u']) {
			mSampler->setReportProgress(!mSampler->getReportProgress());
			mKeyBuffer['u'] = false;
		}

		if (mNeedRender) {
			Update();
			mNeedRender = false;
		} else {
			usleep(50000);
		}
	}

	static void KeyboardSpecDown(int key, int x, int y) {
		appGlut& app = *GetInstance();

		static bool fullScreen = false;

		switch(key) {
			case GLUT_KEY_F5:
				app.mPixelation = 0;
				app.setFrameSize(640, 480);
				app.mSampler->setSamples(3);
				app.mSampler->setReportProgress(true);
				app.mSampler->Render(app.mCamera, app.mGeometry, app.mFrame);
				app.mFrame->Save("./shot.hdr");
				app.mSampler->setReportProgress(false);
				app.setFrameSize(640, 480);
				app.mSampler->setSamples(1);
				break;
			case GLUT_KEY_F4:
				if (!fullScreen) {
					glutFullScreen();
				} else {
					glutPositionWindow(0,0);
				}
				fullScreen = !fullScreen;
				break;
		}
	}

};
