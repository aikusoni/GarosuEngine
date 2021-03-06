#include <GarosuTypedef.h>

#include "GarosuTimer.h"

#include <chrono>

using namespace std;
using namespace std::chrono;

namespace Garosu {

	inline f64 GetCurrentTimeSecond(void) {
		return duration<f64>(high_resolution_clock::now().time_since_epoch()).count();
	}

	GameTimer::GameTimer(void)
		: mDeltaTime(0.0)
		, mPausedTime(0.0)
		, mBeginTime(0.0)
		, mStopTime(0.0)
		, mPrevTime(0.0)
		, mCurrTime(0.0)
		, mStopped(false)
	{
	}

	GameTimer::~GameTimer(void)
	{

	}

	f64 GameTimer::TotalTime(void) const
	{
		if (mStopped)
		{
			return mStopTime - mPausedTime - mBeginTime;
		}
		else
		{
			return mCurrTime - mPausedTime - mBeginTime;
		}
	}

	f64 GameTimer::DeltaTime(void) const
	{
		return mDeltaTime;
	}

	void GameTimer::Reset(void)
	{
		mBeginTime = GetCurrentTimeSecond();
		mCurrTime = mBeginTime;
		mPrevTime = mBeginTime;
		mStopTime = 0.0;
		mPausedTime = 0.0;
		mStopped = false;
	}

	void GameTimer::Start(void)
	{
		if (mStopped)
		{
			auto startTime = GetCurrentTimeSecond();
			mPausedTime += startTime - mStopTime;
			mPrevTime = startTime;

			mStopTime = 0.0;
			mStopped = false;
		}
	}

	void GameTimer::Stop(void)
	{
		mStopTime = GetCurrentTimeSecond();
	}

	void GameTimer::Tick(void)
	{
		if (mStopped)
		{
			mDeltaTime = 0.0;
			return;
		}

		mCurrTime = GetCurrentTimeSecond();
		mDeltaTime = mCurrTime - mPrevTime;
		mPrevTime = mCurrTime;

		if (mDeltaTime < 0.0)
		{
			mDeltaTime = 0.0;
		}
	}

}