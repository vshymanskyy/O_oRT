#include "cMath.h"

namespace cMath{
	real* tCosSin;

	const real Tiny		= real(0.005);

	const real PI		= real(3.1415926535897932384626433832795028841972);

	const real Huge		= real(10000.0);

	const real RadiansToDegrees = real(180.0)/PI;
	const real DegreesToRadians = PI/real(180.0);

	void Init() {
#ifdef USE_TABLE_TRIGONOMETRY
		tCosSin = new real[tblTrigonometryCount];
		for (int i=0; i<tblTrigonometryCount; i++) {
			tCosSin[i] = (real)sin(3.141592653589793238462643383*i/(tblTrigonometryCount/2));
		}
#endif
	}

}
