#include "fsBase.h"
#include "cRandom.h"
#include "fltGaussian.h"
#include "fltMitchell.h"
#include "fltBox.h"

class fsBox
	: public fsBase
{

private:
	int mBoxSize;

public:

	fsBox()
		: mBoxSize(32)
	{
	}


	void InnerRender(camBase* cam, prBase* geom, HdrImage* frame) {
		const real fltSize(3.0);
		fltGaussian flt(vec2(fltSize, fltSize), real(0.3));
		//fltBox flt;

		int samples = 0;
		#pragma omp parallel num_threads(OMP_NR_THREADS)
		{
			const real sample_step = fltSize/real(getSamples());
			const real half_sample_step = sample_step/real(2.0);
			const real sqr_sample_step = sample_step*sample_step;
			const real half_samples = real(getSamples())/real(2.0) + real(0.001);
			const real half_flt_size = fltSize/real(2.0);

			for (int _ex = 0; _ex < frame->Width(); _ex += mBoxSize) {
				for (int _ey = 0; _ey < frame->Height(); _ey += mBoxSize) {
					#pragma omp single nowait
					{
						int ex = _ex;
						int ey = _ey;

						for (int lx = 0; lx < mBoxSize; lx++) {
							const int x = ex + lx;
							if (x >= frame->Width()) {
								break;
							}
							for (int ly = 0; ly < mBoxSize; ly++) {
								const int y = ey + ly;
								if (y >= frame->Height()) {
									break;
								}

								const int pixel_index = y * frame->Width() + x;

								frame->Buffer()[pixel_index] = rgba(0.0);
								channel weight(0.0);

								for (real sx = -half_samples; sx < half_samples; sx += real(1)) {
									for (real sy = -half_samples; sy < half_samples; sy += real(1)) {
										const real sub_x =  sample_step * real(sx) + cRandom::InRange(-half_sample_step, half_sample_step);
										const real sub_y = sample_step * real(sy) + cRandom::InRange(-half_sample_step, half_sample_step);
										const channel curWeight = channel(flt.getWeight(vec2(sub_x, sub_y)));

										Ray ray(geom);
										rgba col;
										cam->getRay(real(0.5 + x) + sub_x, real(0.5 + y) + sub_y, &ray);
										geom->Trace(ray, &col);
										samples++;
										frame->Buffer()[pixel_index] += col * curWeight;
										weight += curWeight;
									}
								}

								int oversampled = -7;
								if (getReportProgress()) {
									int needMore = -oversampled;
									while (weight < channel(512) && needMore > 0) {
										const real sub_x = real(cRandom::InRange(-half_flt_size, half_flt_size));
										const real sub_y = real(cRandom::InRange(-half_flt_size, half_flt_size));
										const channel curWeight = channel(flt.getWeight(vec2(sub_x, sub_y)));

										if (curWeight < 0.0001) {
											continue;
										}

										Ray ray(geom);
										rgba col;
										cam->getRay(real(0.5 + x) + sub_x, real(0.5 + y) + sub_y, &ray);
										geom->Trace(ray, &col);
										samples++;
										oversampled++;

										rgba prev = frame->Buffer()[pixel_index] / weight;

										frame->Buffer()[pixel_index] += col * curWeight;
										weight += curWeight;

										rgba curr = frame->Buffer()[pixel_index] / weight;
										rgba diff = (prev - curr).Abs();

										if (diff.r < 0.0001 && diff.g < 0.0001 && diff.b < 0.0001) {
											needMore--;
										} else {
											needMore++;
										}
									}
								}
								frame->Buffer()[pixel_index] /= weight;
								//if (frameWeight) {
								//	frameWeight->Buffer()[pixel_index] = rgba(weight);
								//}
							}
						}
					}
					const int boxIdx = 1 + _ey / mBoxSize + (_ex / mBoxSize) * (frame->Height() / mBoxSize);
					const int boxQty = (frame->Width() * frame->Height()) / Square(mBoxSize);
					reportProgress((100.0f * boxIdx) / boxQty);
				}
			}
		}
		if (getReportProgress()) {
			frame->setProperty("AvgSamplesPerPixel", float(samples)/(frame->Width()*frame->Height()));
		}
	}
};
