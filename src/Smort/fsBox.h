#include "fsBase.h"
#include "cRandom.h"
#include "fltBox.h"
#include "rgba.h"

#include <GL/glut.h>

class fsBox
	: public fsBase
{

private:
	unsigned mBoxSize;

public:

	fsBox()
		: mBoxSize(16)
	{
	}


	void InnerRender(camBase* cam, prBase* geom, HdrImage* frame) {
		//const real fltSize(1.0);
		//fltGaussian flt(vec2(fltSize, fltSize), real(0.3));
		//fltBox flt;

		int samples = 0;
		const real sample_step = real(1.0)/real(getSamples());
		const real half_sample_step = sample_step/real(2.0);
		const real sqr_sample_step = sample_step*sample_step;
		const real half_samples = real(getSamples())/real(2.0) + real(0.001);
		const real half_flt_size = real(0.5);//fltSize/real(2.0);

		#pragma omp parallel num_threads(OMP_NR_THREADS)
		{
			for (int _ex = 0; _ex < frame->Width(); _ex += mBoxSize) {
				for (int _ey = 0; _ey < frame->Height(); _ey += mBoxSize) {
					#pragma omp single nowait
					{
						int ex = _ex;
						int ey = _ey;

						const int xend = Min(mBoxSize, frame->Width() - ex);
						for (int lx = 0; lx < xend; lx++) {
							const int x = ex + lx;
							const int yend = Min(mBoxSize, frame->Height() - ey);
							for (int ly = 0; ly < yend; ly++) {
								const int y = ey + ly;

								rgba& pixel = frame->Buffer()[y * frame->Width() + x];

								pixel = rgba(0.0);
								channel weight(0.0);

								Ray ray(geom);
								rgba col;

								for (real sx = -half_samples; sx < half_samples; sx += real(1)) {
									for (real sy = -half_samples; sy < half_samples; sy += real(1)) {
										const real sub_x =  sample_step * real(sx) + cRandom::InRange(-half_sample_step, half_sample_step);
										const real sub_y = sample_step * real(sy) + cRandom::InRange(-half_sample_step, half_sample_step);
										//const channel curWeight = channel(flt.getWeight(vec2(sub_x, sub_y)));

										cam->getRay(real(0.5 + x) + sub_x, real(0.5 + y) + sub_y, &ray);
										ray.Clear();
										geom->Trace(ray, &col);
										samples++;
										pixel += col;// * curWeight;
										weight += 1.0;//curWeight;
									}
								}

								/*int oversampled = -7;
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
								}*/
								pixel /= weight;
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

class fsLine
	: public fsBase
{
public:

	fsLine()
		: mOffsetX (0)
		, mOffsetY (0)
		, mModX (1)
		, mModY (1)
	{
	}


	void InnerRender(camBase* cam, prBase* geom, HdrImage* frame) {
		mOffsetX = ++mOffsetX % mModX;
		if(!mOffsetX) {
			mOffsetY = ++mOffsetY % mModY;
		}
		#pragma omp parallel num_threads(OMP_NR_THREADS)
		{
			for (int x = mOffsetX; x < frame->Width(); x+=mModX) {
				#pragma omp single nowait
				{
					for (int y = mOffsetY; y < frame->Height(); y+=mModY) {
						rgba& pixel = frame->Buffer()[y * frame->Width() + x];

						pixel = rgba(0.0f);

						Ray ray(geom);
						cam->getRay(real(0.5 + x), real(0.5 + y), &ray);
						rgba color;
						geom->Trace(ray, &pixel);

						//float exposure = -15.00f;
						//pixel = rgba(1) - (pixel * exposure).Exp();
					}
				}
			}
		}
	}
private:
	int mOffsetX, mOffsetY, mModX, mModY;
};

