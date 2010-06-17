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

	camThinLens* mCamera;
	prBase* mGeometry;
	fsBase* mSampler;
	HdrImage* mFrame;

public:

	appGlut() {}
	~appGlut() {}

	static appGlut* GetInstance() {
		static appGlut* inst = new appGlut();
		return inst;
	}

	void Run(int argc, char** argv, fsBase* sampler, camThinLens* cam, prBase* geom) {
		mPixelation	= 1;
		mWidth		= 640;
		mHeight		= 480;
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
	}

	void Update(bool resize = true) {
		const int factor = 1 << mPixelation;
		if (resize) {
			setFrameSize(mWidth/factor, mHeight/factor);
		} else {
			glutReshapeWindow(mFrame->Width()*factor, mFrame->Height()*factor);
		}
		Render();
		glPixelZoom (factor, factor);
		glDrawPixels(mFrame->Width(), mFrame->Height(), GL_RGBA, GL_FLOAT, mFrame->Buffer());
		glutSwapBuffers();
		//printf("%d x %d, %d spp: %1.3f, %f RPS\n", mFrame->Width(), mFrame->Height(), mSampler->getSamples(), 1.0/mFrame->RenderTime(), mFrame->RPS());
	}

	static void Display() {
		appGlut& app = *GetInstance();
		const int factor = 1 << app.mPixelation;
		glPixelZoom (factor, factor);
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
			sprintf(buff, "smort [%d x %d]\n", width, height);
			glutSetWindowTitle(buff);
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
		appGlut& app = *GetInstance();

		bool changed = false;
		if (app.mKeyBuffer[0] || app.mKeyBuffer[1] || app.mKeyBuffer[2]) {
			changed = true;
		}
		if (app.mKeyBuffer['w']) {
			app.mCamera->mPosition += app.mCamera->mBase.w*real(0.5); changed = true;
		}
		if (app.mKeyBuffer['s']) {
			app.mCamera->mPosition -= app.mCamera->mBase.w*real(0.5); changed = true;
		}
		if (app.mKeyBuffer['a']) {
			app.mCamera->mPosition -= app.mCamera->mBase.u*real(0.5); changed = true;
		}
		if (app.mKeyBuffer['d']) {
			app.mCamera->mPosition += app.mCamera->mBase.u*real(0.5); changed = true;
		}
		if (app.mKeyBuffer['r']) {
			app.mCamera->mPosition += app.mCamera->mBase.v*real(0.5); changed = true;
		}
		if (app.mKeyBuffer['f']) {
			app.mCamera->mPosition -= app.mCamera->mBase.v*real(0.5); changed = true;
		}
		//speed
		if (app.mKeyBuffer['W']) {
			app.mCamera->mPosition += app.mCamera->mBase.w*real(5.0); changed = true;
		}
		if (app.mKeyBuffer['S']) {
			app.mCamera->mPosition -= app.mCamera->mBase.w*real(5.0); changed = true;
		}
		if (app.mKeyBuffer['A']) {
			app.mCamera->mPosition -= app.mCamera->mBase.u*real(5.0); changed = true;
		}
		if (app.mKeyBuffer['D']) {
			app.mCamera->mPosition += app.mCamera->mBase.u*real(5.0); changed = true;
		}
		if (app.mKeyBuffer['R']) {
			app.mCamera->mPosition += app.mCamera->mBase.v*real(5.0); changed = true;
		}
		if (app.mKeyBuffer['F']) {
			app.mCamera->mPosition -= app.mCamera->mBase.v*real(5.0); changed = true;
		}
		//projection
		if (app.mKeyBuffer['q']) {
			app.mCamera->mDistance -= 0.1;
			changed = true;
		}
		if (app.mKeyBuffer['e']) {
			app.mCamera->mDistance += 0.1;
			changed = true;
		}
		//super sampling
		if (app.mKeyBuffer['g']) {
			if (app.mSampler->getSamples() > 1) {
				app.mSampler->setSamples(app.mSampler->getSamples()-1);
				changed = true;
				app.mKeyBuffer['g'] = false;
			}
		}
		if (app.mKeyBuffer['h']) {
			if (app.mSampler->getSamples() < 5) {
				app.mSampler->setSamples(app.mSampler->getSamples()+1);
				changed = true;
				app.mKeyBuffer['h'] = false;
			}
		}
		//DoF
		if (app.mKeyBuffer['p']) {
			app.mCamera->mUseDoF = !app.mCamera->mUseDoF;
			app.mKeyBuffer['p'] = false;
			changed = true;
		}
		if (app.mKeyBuffer['[']) {
			app.mCamera->mDofDistance -= 0.05;
			changed = true;
		}
		if (app.mKeyBuffer[']']) {
			app.mCamera->mDofDistance += 0.05;
			changed = true;
		}
		if (app.mKeyBuffer['-']) {
			if (app.mCamera->mDofAperture > real(0)) {
				app.mCamera->mDofAperture -= 0.01;
				changed = true;
			}
		}
		if (app.mKeyBuffer['=']) {
			app.mCamera->mDofAperture += 0.01;
			changed = true;
			usleep(50000);
		}
		if (app.mKeyBuffer['b']) {
			app.mSampler->setPostProcess(!app.mSampler->getPostProcess());
			changed = true;
			app.mKeyBuffer['b'] = false;
		}


		if (app.mKeyBuffer['n']) {
			if (app.mPixelation > 0) {
				app.mPixelation--;
				changed = true;
				app.mKeyBuffer['n'] = false;
			}
		}
		if (app.mKeyBuffer['m']) {
			if (app.mPixelation < 4) {
				app.mPixelation++;
				changed = true;
				app.mKeyBuffer['m'] = false;
			}
		}

		if (app.mKeyBuffer['t'] || app.mKeyBuffer['T']) {
			SKEL = new skel();
			const int factor = 1 << app.mPixelation;
			if (app.mKeyBuffer['t']) {
				//for (int i = 0; i < 512; i++) {
					Ray ray(app.mGeometry);
					rgba col;
					app.mCamera->getRay(app.mPosX/factor, app.mPosY/factor, &ray);
					app.mGeometry->Trace(ray, &col);
				//}
			} else if (app.mKeyBuffer['T']) {
				app.Render();
			}
			SKEL->save("trace.skel");
			printf("Trace saved (%d lines)\n", SKEL->getLineCount());
			delete SKEL;
			SKEL = NULL;
			app.mCamera->SaveSkel();
			app.mKeyBuffer['t'] = false;
			app.mKeyBuffer['T'] = false;
		}

		if (app.mKeyBuffer['.']) {
			if (NR_BOUNCES<20) {
				NR_BOUNCES ++;
				changed = true;
				app.mKeyBuffer['.'] = false;
			}
		}
		if (app.mKeyBuffer[',']) {
			if (NR_BOUNCES>1) {
				NR_BOUNCES --;
				changed = true;
				app.mKeyBuffer[','] = false;
			}
		}

		if (changed) {
			app.Update();
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
