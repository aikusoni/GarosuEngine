#pragma once

class GameTimer final
{
public:
	GameTimer(void);
	GameTimer(const GameTimer&) = delete;
	GameTimer& operator=(const GameTimer&) = delete;

	virtual ~GameTimer(void);

public:
	double TotalTime(void) const;
	double DeltaTime(void) const;

	void Reset(void);
	void Start(void);
	void Stop(void);

	void Tick(void);

private:
	double mDeltaTime;

	double mPausedTime;
	double mBeginTime;
	double mStopTime;
	double mPrevTime;
	double mCurrTime;

	bool mStopped;
};