#pragma once

#ifdef WINDOWS
	#include <windows.h>
#else
	#include <time.h>

static
timespec
diff_timespec(timespec& start, timespec& end)
{
    struct timespec result;
    if (end.tv_nsec < start.tv_nsec) {
    	// perform carry like in normal subtraction
        //                123456789
        result.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
        result.tv_sec = end.tv_sec - 1 - start.tv_sec;
    } else {
        result.tv_nsec = end.tv_nsec - start.tv_nsec;
        result.tv_sec = end.tv_sec - start.tv_sec;
    }
    return result;
}



#endif

class PreciseTimer
{
public:
	void Start();
	void Stop();
	double GetDurationInSecs();

private:

#ifdef WINDOWS
	LARGE_INTEGER mTimeStart;
	LARGE_INTEGER mTimeStop;
#else
	timespec mTimeStart;
	timespec mTimeStop;
#endif

};

#ifdef WINDOWS

	inline void PreciseTimer::Start() {
		QueryPerformanceCounter(&mTimeStart);
	}

	inline void PreciseTimer::Stop() {
		QueryPerformanceCounter(&mTimeStop);
	}

	inline double PreciseTimer::GetDurationInSecs() {
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		double duration = (double)(mTimeStop.QuadPart-mTimeStart.QuadPart)/(double)freq.QuadPart;
		return duration;
	}

#else

	inline void PreciseTimer::Start() {
		clock_gettime(CLOCK_REALTIME, &mTimeStart);
	}

	inline void PreciseTimer::Stop() {
		clock_gettime(CLOCK_REALTIME, &mTimeStop);
	}

	inline double PreciseTimer::GetDurationInSecs() {
		timespec elapsed = diff_timespec(mTimeStart, mTimeStop);
		return (double)elapsed.tv_sec + (double)elapsed.tv_nsec/1000000000.0;
	}

#endif
