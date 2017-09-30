#pragma once
#ifndef __GAROSU_TIMER_H__
#define __GAROSU_TIMER_H__

namespace Garosu
{

	class GameTimer final
	{
	public:
		GameTimer(void);
		GameTimer(const GameTimer&) = delete;
		GameTimer& operator=(const GameTimer&) = delete;

		virtual ~GameTimer(void);

	public:
		f64 TotalTime(void) const;
		f64 DeltaTime(void) const;

		void Reset(void);
		void Start(void);
		void Stop(void);

		void Tick(void);

	private:
		f64 mDeltaTime;

		f64 mPausedTime;
		f64 mBeginTime;
		f64 mStopTime;
		f64 mPrevTime;
		f64 mCurrTime;

		bool mStopped;
	};

}

#endif