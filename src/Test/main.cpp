
#include "appGlut.h"

#include <signal.h>

#include "cRandom.h"
#include "cMath.h"

#include "sceneBruteforce.h"
#include "camThinLens.h"

#include "fsBase.h"
#include "fsBox.h"

#include "prSphere.h"
#include "prTriangle.h"

#include "shConst.h"
#include "envConst.h"

#include "envCubic.h"
#include "envSpheric.h"

#include "shDepth.h"
#include "shNormal.h"
#include "shCosND.h"
#include "shTrace.h"
#include "shPartialU.h"
#include "shPartialV.h"

#include "modNormal.h"
#include "modGlossy.h"
#include "modScene.h"
#include "modBilinearUV.h"
#include "modAffineUV.h"

#include "shCheckered.h"
#include "shWavelet.h"
#include "shPerlin.h"
#include "shPerlinImproved.h"
#include "shMarble.h"
#include "shWood.h"
#include "shHdrImage.h"
#include "shMirror.h"
#include "shGlass.h"

#include "shAbs.h"
#include "shAdd.h"
#include "shSub.h"
#include "shMul.h"
#include "shDiv.h"
#include "shAddN.h"
#include "shMulN.h"

#include "shTurbulence.h"
#include "shBlend.h"
#include "shDeGamma.h"
#include "shDeExposure.h"

const int OMP_NR_THREADS = 2;

int STAT_RAYS = 0;
int NR_BOUNCES = 7;
class skel* SKEL = NULL;

void addQuad(sceneBase* scene, const vec3& a, const vec3& b, const vec3& c, const vec3& d, const shBase* shFront, const shBase* shBack) {
	scene->Add(
		new prTriangle(
			a, b, c,
			vec2( 1,  1),
			vec2(-1,  1),
			vec2(-1, -1),
			shFront, shBack
		)
	);

	scene->Add(
		new prTriangle(
			b, d, c,
			vec2( 1, -1),
			vec2( 1,  1),
			vec2(-1, -1),
			shFront, shBack
		)
	);
}


void addBox(sceneBase* scene, const vec3& min, const vec3& max, const shBase* shFront, const shBase* shBack) {
	vec3 a( min.x, min.y, min.z);
	vec3 b( min.x, min.y, max.z);
	vec3 c( max.x, min.y, min.z);
	vec3 d( max.x, min.y, max.z);

	vec3 e( min.x, max.y, min.z);
	vec3 f( min.x, max.y, max.z);
	vec3 g( max.x, max.y, min.z);
	vec3 h( max.x, max.y, max.z);

	addQuad(scene, a, c, b, d, shFront, shBack);
	addQuad(scene, e, f, g, h, shFront, shBack);

	addQuad(scene, a, e, c, g, shFront, shBack);
	addQuad(scene, b, d, f, h, shFront, shBack);

	addQuad(scene, f, e, b, a, shFront, shBack);
	addQuad(scene, g, h, c, d, shFront, shBack);
}


void addTetraHedron(sceneBase* scene, const vec3& a, const vec3& b, const vec3& c, const vec3& d, const shBase* shFront, const shBase* shBack) {
	scene->Add(
		new prTriangle(
			a, b, c,
			vec2( 1,  1),
			vec2(-1,  1),
			vec2(-1, -1),
			shFront, shBack
		)
	);
	scene->Add(
		new prTriangle(
			a, c, d,
			vec2( 1, -1),
			vec2( 1,  1),
			vec2(-1, -1),
			shFront, shBack
		)
	);
	scene->Add(
		new prTriangle(
			a, d, b,
			vec2( 1, -1),
			vec2( 1,  1),
			vec2(-1, -1),
			shFront, shBack
		)
	);
	scene->Add(
		new prTriangle(
			b, d, c,
			vec2( 1, -1),
			vec2( 1,  1),
			vec2(-1, -1),
			shFront, shBack
		)
	);
}

HdrImage* frame;

void onAbort(int param) {
	printf("\nRender canceled.\n");
	frame->Save("./abort.hdr");
	exit(0);
}

int main(int argc, char** args) {
	cMath::Init();
	cRandom::Init();

    signal(SIGABRT, onAbort);
    signal(SIGTERM, onAbort);
    signal(SIGINT,  onAbort);

	sceneBruteForce* a = new sceneBruteForce(
//		/new envConst(1.0)
		//new envCubic(new shConst(1.0, 0.0, 0.0), new shConst(0.0, 1.0, 0.0), new shConst(0.0, 0.0, 1.0), new shConst(1.0, 1.0, 0.0), new shConst(1.0, 0.0, 1.0), new shConst(0.0, 1.0, 1.0))
		new envSpheric(new shHdrImage("./rnl.hdr"))
	);
/*
	a->Add(
		new prSphere(
			vec3(0.0, 9.0, 0.0), 2.0,
			new shMirror(new shConst(0.8, 0.8, 0.8))
		)
	);

	a->Add(
		new prSphere(
			vec3(0.0, 4.5, 0.0), 2.0,
			new shAbs(new shNormal())
		)
	);
*/
	/*a->Add(
		new prSphere(
			vec3(4.0, 0.0, 0.0), 2.0,
			new shCheckered(0.1, 0.1)
		)
	);*/


/*
	a->Add(
		new prSphere(
			vec3(0.0, 0.0, 2.0), 1.0,
			//new shGlass(new shConst(0.85, 0.85, 0.85), new shAdd(new shAbs(new shPerlin()), new shConst(1.0)))
			new shGlass(new shConst(0.8, 0.8, 0.8), new shConst(1.33))
			//new shDiv(new shConst(1.0), new shDepth())
			//new shMirror(new shConst(0.8, 0.8, 0.8))

			//new shMirror(new shCheckered(0.05, 0.05))

			, true, false
		)
	);

	a->Add(
		new prSphere(
			vec3(0.0, 0.0, -2.0), 1.0,
			//new shGlass(new shConst(0.85, 0.85, 0.85), new shAdd(new shAbs(new shPerlin()), new shConst(1.0)))
			new shGlass(new shConst(0.8, 0.8, 0.8), new shConst(1.33))
			//new shDiv(new shConst(1.0), new shDepth())
			//new shMirror(new shConst(0.8, 0.8, 0.8))

			//new shMirror(new shCheckered(0.05, 0.05))

			, true, false
		)
	);
*/
/*
	a->Add(
		new prSphere(
			vec3(-1.5, 0.0, 0.0), 2.0,
			//new shGlass(new shConst(0.85, 0.85, 0.85), new shAdd(new shAbs(new shPerlin()), new shConst(1.0)))
			new shGlass(new shConst(0.8, 0.8, 0.8), new shConst(1.33))
			//new shDiv(new shConst(1.0), new shDepth())
			//new shMirror(new shConst(0.8, 0.8, 0.8))
			, false, true
		)
	);
*/
	/*
	a->Add(
		new prTriangle(
			vec3(10.0, 10.0, 3.0),
			vec3(-10.0, 10.0, 3.0),
			vec3(-10.0, -10.0, 3.0),
			vec2(0.0, 0.0),
			vec2(0.0, 0.0),
			vec2(0.0, 0.0),
			new modGlossy(new shMirror(new shConst(0.8, 0.8, 0.8)), new shConst(0.05))
		)
	);

	a->Add(
		new prTriangle(
			vec3(10.0, 10.0, 3.0),
			vec3(10.0, -10.0, 3.0),
			vec3(-10.0, -10.0, 3.0),
			vec2(0.0, 0.0),
			vec2(0.0, 0.0),
			vec2(0.0, 0.0),
			new modGlossy(new shMirror(new shConst(0.8, 0.8, 0.8)), new shConst(0.05))
		)
	);

	a->Add(
		new prTriangle(
			vec3(10.0, 10.0, -3.0),
			vec3(-10.0, 10.0, -3.0),
			vec3(-10.0, -10.0, -3.0),
			vec2(0.0, 0.0),
			vec2(0.0, 0.0),
			vec2(0.0, 0.0),
			new modGlossy(new shMirror(new shConst(0.8, 0.8, 0.8)), new shConst(0.05))
		)
	);

	a->Add(
		new prTriangle(
			vec3(10.0, 10.0, -3.0),
			vec3(10.0, -10.0, -3.0),
			vec3(-10.0, -10.0, -3.0),
			vec2(0.0, 0.0),
			vec2(0.0, 0.0),
			vec2(0.0, 0.0),
			new modGlossy(new shMirror(new shConst(0.8, 0.8, 0.8)), new shConst(0.05))
		)
	);
*/
/*
	a->Add(
		new prSphere(
			vec3(0.0, 0.0, 0.0), 1.5,
			//new shGlass(new shConst(0.85, 0.85, 0.85), new shAdd(new shAbs(new shPerlin()), new shConst(1.0)))
			new shGlass(new shConst(0.8, 0.8, 0.8), new shBlend(new shConst(1.3), new shConst(1.7), new shAbs(new shPerlinImproved())))


			//new shDiv(new shConst(1.0), new shDepth())
			//new shMirror(new shConst(0.8, 0.8, 0.8))

			//new shMirror(new shCheckered(0.05, 0.05))

			, true, false
		)
	);
*/

	//shBase* sh = new shAbs(new shCosND());
	shBase* sh1 = new shGlass(new shConst(0.75, 0.85, 0.85), new shConst(1.3), new shConst(0.5), new shConst(1.5));
	shBase* sh2 = new shGlass(new shConst(1.0), new shConst(1.0));

	//addTetraHedron(a, vec3( 0, 6, 0), vec3(-4, 0.1, -4), vec3( 0, 0.1,  4), vec3( 4, 0.1, -4), sh1, sh1);
	//addTetraHedron(a, vec3( 0, 4, 0), vec3(-2, 1.0, -2), vec3( 0, 1.0,  2), vec3( 2, 1.0, -2), sh2, sh2);

	a->Add(new prSphere(vec3(0.0, 0.5, 0.0), 2.0, sh1, true, true));
	//a->Add(new prSphere(vec3(0.0, 0.5, 0.0), 1.0, sh2, true, true));

	//addBox(a, vec3(-2.0, -2.0, -2.0), vec3(2.0, 2.0, 2.0), sh1, sh1);
	//addBox(a, vec3(-1.8, -1.8, -1.8), vec3(1.8, 1.8, 1.8), sh2, sh2);


	//new shMirror(new shBlend(new shConst(0.8), new shConst(0.4), new shCheckered(1.0, 1.0)))

	//shBase* shColorChecks = new shBlend(new shConst(0.8, 1.0, 0.0), new shConst(0.5, 0.0, 0.5), new shCheckered(1.0, 1.0));
	shBase* shColorChecks = new shCheckered(1.0, 1.0);
	shBase* shMirrorColorChecks = new shMirror(shColorChecks);
	shBase* shGlossyMirrorColorChecks = new modGlossy(shMirrorColorChecks, new shConst(0.1));

	//shBase* shFloor = new shHdrImage("shot.hdr");


	shBase* shFloor = shMirrorColorChecks;

	real size(1000);
	a->Add(
		new prTriangle(
			vec3(-size, -2.0,  size),
			vec3( size, -2.0,  size),
			vec3(-size, -2.0, -size),
			vec2(-size,  size),
			vec2( size,  size),
			vec2(-size, -size),
			shFloor, NULL
		)
	);

	a->Add(
		new prTriangle(
			vec3( size, -2.0,  size),
			vec3( size, -2.0, -size),
			vec3(-size, -2.0, -size),
			vec2( size,  size),
			vec2( size, -size),
			vec2(-size, -size),
			shFloor, NULL
		)
	);

//	a->Precompute();

	camThinLens* cam = new camThinLens(
		vec3(-10.0, 4.0, 4.0),
		vec3(1.0, -0.5, -0.5).normalized(),
		vec3(0.0, 1.0, 0.0).normalized(),
		1.5, 1.0, 0.75
	);
/*
#ifdef DEBUG
		128, 128,
#else
		//128, 128,
		//256, 256,
		//512, 512,
		640, 480,
		//1024, 768,
		//1280, 1024,
#endif
*/
	//cam->SetGeometry(a);
	appGlut::GetInstance()->Run(argc, args, new fsBox(), cam, a);

	return 0;

	int width = 1280;
	int height = 1024;

	frame = new HdrImage(width, height);
	fsBase* sampler = new fsBox();

	cam->setFrameSize(width, height);

	sampler->setSamples(2);
	sampler->setReportProgress(true);
	sampler->Render(cam, a, frame);

	frame->Save("./shot.hdr");
}
