#pragma once

#include "camBase.h"
#include "prBase.h"
#include "HdrImage.h"
#include "PreciseTimer.h"
#include "ppBase.h"

class fsBase {

private:
	int mSamples;
	bool mReportProgress;
	bool mPostProcess;
	List<ppBase*> mPostProcessors;
	PreciseTimer mTimer;

	void (*mDisplayFunc)(void);

	virtual void InnerRender(camBase* cam, prBase* geom, HdrImage* frame) = 0;

protected:

	void reportProgress(float progress) {
		if (getReportProgress()) {
			static float prev = progress - 100.0f;
			static float prevDisp = progress - 100.0f;
			if (fabs(progress - prevDisp) >= 5.0f && mDisplayFunc) {
				mDisplayFunc();
				SleepMs(50);
				prevDisp = progress;
			}

			if (fabs(progress - prev) >= 0.2f) {
				mTimer.Stop();
				printf("Rendering... %3.2f%% [%3.3f seconds]\r", progress, mTimer.GetDurationInSecs());
				fflush(stdout);
				prev = progress;
			}
		}
	}

public:
	fsBase()
		: mSamples			(1)
		, mReportProgress	(false)
		, mPostProcess		(false)
		, mDisplayFunc		(NULL)
	{}

	virtual ~fsBase() {}

    bool getPostProcess() const {
        return mPostProcess;
    }

    List<ppBase*>& getPostProcessors() {
        return mPostProcessors;
    }

    bool getReportProgress() const {
        return mReportProgress;
    }

    int getSamples() const {
        return mSamples;
    }

    void setPostProcess(bool mPostProcess) {
        this->mPostProcess = mPostProcess;
    }

    void setDisplayFunc(void (*func)(void)) {
        this->mDisplayFunc = func;
    }

    void setReportProgress(bool mReportProgress) {
        this->mReportProgress = mReportProgress;
    }

    void setSamples(int mSamples) {
        this->mSamples = mSamples;
    }

    void Render(camBase* cam, prBase* geom, HdrImage* frame) {
		mTimer.Start();
		geom->Precompute();
		cam->Precompute();
		STAT_RAYS = 0;
		InnerRender(cam, geom, frame);
		if (mPostProcess) {
			for (List<ppBase*>::Iterator i = mPostProcessors.First(); i != mPostProcessors.End(); ++i) {
				mPostProcessors[i]->Process(frame);
			}
		}
		mTimer.Stop();
		const double time = mTimer.GetDurationInSecs();

		frame->setProperty("GAMMA", 1.0);
		frame->setProperty("EXPOSURE", 1.0);
		frame->setProperty("RenderTime", time);
		frame->setProperty("TotalRays", double(STAT_RAYS));
		frame->setProperty("RaysPerSec", double(STAT_RAYS)/time);
		frame->setProperty("Threads", OMP_NR_THREADS);
		frame->setProperty("Recursion", NR_BOUNCES);
	}
};
