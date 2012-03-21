
#include "appGlut.h"

#include <signal.h>

#include "cRandom.h"
#include "cMath.h"

#include "sceneBase.h"
#include "camThinLens.h"

#include "fsBase.h"
#include "fsBox.h"

#include "prSphere.h"
#include "prTriangle.h"

#include "envConst.h"

#include "envCubic.h"
#include "envSpheric.h"

#include "shDepth.h"
#include "shNormal.h"
#include "shCosND.h"
#include "shTrace.h"
#include "shPartialU.h"
#include "shPartialV.h"
#include "shReflect.h"
#include "shRefract.h"

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

const int OMP_NR_THREADS = 4;

int STAT_RAYS = 0;
int NR_BOUNCES = 10;
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

const double CONST_D = 10;

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
	//	new envConst(1.0)
	//	new envCubic(new shConst(1.0, 0.0, 0.0), new shConst(0.0, 1.0, 0.0), new shConst(0.0, 0.0, 1.0), new shConst(1.0, 1.0, 0.0), new shConst(1.0, 0.0, 1.0), new shConst(0.0, 1.0, 1.0))
		new envSphericLanLon(new shHdrImage("/media/Transcend/Resources/environments/equirectangular/OpenfootageNET_Beach_CavalinoItalyHigh.hdr"))
	);


	const float scale = 7.0;
	const float radius = 3.0;
	const vec3 centers[27] = {
			{0.0*scale,  0.0*scale, 0.0*scale},
			{0.0*scale,  0.0*scale, 1.0*scale},
			{0.0*scale,  0.0*scale, 2.0*scale},
			{0.0*scale,  1.0*scale, 0.0*scale},
			{0.0*scale,  1.0*scale, 1.0*scale},
			{0.0*scale,  1.0*scale, 2.0*scale},
			{0.0*scale,  2.0*scale, 0.0*scale},
			{0.0*scale,  2.0*scale, 1.0*scale},
			{0.0*scale,  2.0*scale, 2.0*scale},
			{1.0*scale,  0.0*scale, 0.0*scale},
			{1.0*scale,  0.0*scale, 1.0*scale},
			{1.0*scale,  0.0*scale, 2.0*scale},
			{1.0*scale,  1.0*scale, 0.0*scale},
			{1.0*scale,  1.0*scale, 1.0*scale}, //center
			{1.0*scale,  1.0*scale, 2.0*scale},
			{1.0*scale,  2.0*scale, 0.0*scale},
			{1.0*scale,  2.0*scale, 1.0*scale},
			{1.0*scale,  2.0*scale, 2.0*scale},
			{2.0*scale,  0.0*scale, 0.0*scale},
			{2.0*scale,  0.0*scale, 1.0*scale},
			{2.0*scale,  0.0*scale, 2.0*scale},
			{2.0*scale,  1.0*scale, 0.0*scale},
			{2.0*scale,  1.0*scale, 1.0*scale},
			{2.0*scale,  1.0*scale, 2.0*scale},
			{2.0*scale,  2.0*scale, 0.0*scale},
			{2.0*scale,  2.0*scale, 1.0*scale},
			{2.0*scale,  2.0*scale, 2.0*scale},
	};


	int pos = 0;
	/*a->Add(new prSphere(centers[pos++], radius, new shMirror(new shConst(0.8))));
	a->Add(new prSphere(centers[pos++], radius, new shReflect(new shConst(0.8))));
	a->Add(new prSphere(centers[pos++], radius, new shTrace(new shNormal())));
	a->Add(new prSphere(centers[pos++], radius, new shCheckered(1.0, 0.05)));
	a->Add(new prSphere(centers[pos++], radius, new shCheckered(0.5, 0.5, 0.5)));
	a->Add(new prSphere(centers[pos++], radius, new shCosND()));
	a->Add(new prSphere(centers[pos++], radius, new shDiv(new shConst(5.0), new shDepth())));
	a->Add(new prSphere(centers[pos++], radius, new shGlass(new shConst(0.8), new shConst(1.33), new shConst(0.0), new shConst(100))));
	a->Add(new prSphere(centers[pos++], radius, new shRefract(new shConst(0.8), new shConst(1.33))));
	a->Add(new prSphere(centers[pos++], radius, new shPerlinImproved()));
	a->Add(new prSphere(centers[pos++], radius, new shPerlin()));
	//a->Add(new prSphere(centers[pos++], radius, new shWavelet()));
	a->Add(new prSphere(centers[pos++], radius, new shAbs(new shNormal())));
	a->Add(new prSphere(centers[pos++], radius, new shTurbulence(new shCheckered(0.1, 0.1), 10, 1, 0.5)));
	a->Add(new prSphere(centers[pos++], radius, new shWood(new shPerlinImproved(), 0, 15, 2)));
	a->Add(new prSphere(centers[pos++], radius, new shMarble(new shPerlinImproved(), 0, 4, 3)));
	//a->Add(new prSphere(centers[pos++], radius, new shBlend(new shConst(1.0), new shReflect(new shConst(0.8)), new shPerlinImproved())));
	a->Add(new prSphere(centers[pos++], radius, new shGlass(new shConst(0.8), new shAdd(new shAbs(new shPerlin()), new shConst(1.0)))));
	//a->Add(new prSphere(centers[pos++], radius, new modGlossy(new shMirror(new shConst(0.8)), new shConst(0.05))));*/
	//a->Add(new prSphere(centers[pos++], radius, new shGlass(new shConst(0.8, 0.8, 0.8), new shBlend(new shConst(1.3), new shConst(1.7), new shAbs(new shPerlinImproved())))));
	//a->Add(new prSphere(centers[pos++], radius, new shGlass(new shConst(0.85, 0.85, 0.85), new shAdd(new shAbs(new shPerlin()), new shConst(1.0)))));
	//a->Add(new prSphere(centers[pos++], radius, new shMirror(new shBlend(new shConst(0.8), new shConst(0.4), new shCheckered(1.0, 1.0)))));

	shBase* glass = new shGlass(new shConst(1.0, 1.0, 1.0), new shConst(1.7));

	for(int i=0; i<27; i++)
		a->Add(new prSphere(centers[pos++], radius, glass, glass));

	//a->Add(new prSphere(centers[pos++], radius, new shReflect(new shConst(0.05))));


	//shBase* sh1 = new shGlass(new shConst(0.75, 0.85, 0.85), new shConst(1.3), new shConst(0.5), new shConst(1.5));
	//shBase* sh2 = new shGlass(new shConst(1.0), new shConst(1.0));

	//addTetraHedron(a, vec3( 0, 6, 0), vec3(-4, 0.1, -4), vec3( 0, 0.1,  4), vec3( 4, 0.1, -4), sh1, sh1);
	//addTetraHedron(a, vec3( 0, 4, 0), vec3(-2, 1.0, -2), vec3( 0, 1.0,  2), vec3( 2, 1.0, -2), sh2, sh2);

	//addBox(a, vec3(-2.0, -2.0, -2.0), vec3(2.0, 2.0, 2.0), sh1, sh1);
	//addBox(a, vec3(-1.8, -1.8, -1.8), vec3(1.8, 1.8, 1.8), sh2, sh2);



	//shBase* shColorChecks = new shBlend(new shConst(0.8, 1.0, 0.0), new shConst(0.5, 0.0, 0.5), new shCheckered(1.0, 1.0));
	shBase* shColorChecks = new shCheckered(1.0, 1.0);
	shBase* shMirrorColorChecks = new shMirror(shColorChecks);
	shBase* shGlossyMirrorColorChecks = new modGlossy(shMirrorColorChecks, new shConst(0.1));




	//tshConst2< tshConst<CONST_D, CONST_D, CONST_D> > shader_;

	//shBase* shFloor = new shGlass(new shConst(0.8), new shConst(1.33), new shConst(0.0), new shConst(100));
	//shBase* shFloor = new shAbs(new shNormal());
	//shBase* shFloor = new shCheckered(.1, .1);

	//real size(20);
	//addBox(a, vec3(0, -size,  0), vec3(size, -4, size), shFloor, shFloor);


	//shBase* shBox = new shGlass(new shConst(0.7), new shAdd(new shConst(1.1), new shPerlin()));
	//addBox(a, vec3(-5, -5, -5), vec3(0, 0, 0), shBox, shBox);


//	a->Precompute();

	camThinLens* cam = new camThinLens(
		vec3(-10.0, 4.0, 4.0),
		vec3(1.0, -0.5, -0.5).normalized(),
		vec3(0.0, 1.0, 0.0).normalized(),
		1.5, 1.0, 1.0
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
	appGlut::GetInstance()->Run(argc, args, new fsLine(), cam, a);

	return 0;

	int width = 1280;
	int height = 1280;

	frame = new HdrImage(width, height);
	fsBase* sampler = new fsBox();

	cam->setFrameSize(width, height);

	sampler->setSamples(4);
	sampler->setReportProgress(true);
	sampler->Render(cam, a, frame);

	frame->Save("./shot.hdr");
}
