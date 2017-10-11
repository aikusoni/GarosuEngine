#include <GarosuTypedef.h>

#include "GarosuThread.h"

#include <thread>
#include <chrono>
#include <atomic>

namespace Garosu
{

	static void DoWorkCaller(BaseThread* thr)
	{
		thr->DoWork();
	}

	class BaseThread::impl
	{
	public:
		std::thread thr;
	};

	BaseThread::BaseThread(void)
		: pImpl(mk_uptr<impl>())
	{

	}

	BaseThread::~BaseThread(void)
	{

	}

	void BaseThread::Start(void)
	{
		pImpl->thr = std::thread(DoWorkCaller, this);
	}

	void BaseThread::Join(void)
	{
		if (pImpl->thr.joinable())
			pImpl->thr.join();
	}


	/*
	*
	*
	*
	*/
	class LoopThread::impl
	{
	public:
		std::atomic<bool> NeedStop = false;
	};

	LoopThread::LoopThread(void)
		: pImpl(mk_uptr<impl>())
	{

	}

	LoopThread::~LoopThread(void)
	{

	}

	void LoopThread::Start(void)
	{
		pImpl->NeedStop = false;
		BaseThread::Start();
	}

	void LoopThread::Stop(void)
	{
		pImpl->NeedStop = true;
	}

	void LoopThread::DoWork(void)
	{
		if (!OnBegin()) return;
		while (true)
		{
			if (pImpl->NeedStop) break;
			if (!OnLoop()) break;
		}
		if (!OnEnd()) return;
	}

	void ThreadUtils::SleepFor(u32 microSeconds)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(microSeconds));
	}

	u32 ThreadUtils::GetConcurrencyCount(void)
	{
		return std::thread::hardware_concurrency();
	}

}