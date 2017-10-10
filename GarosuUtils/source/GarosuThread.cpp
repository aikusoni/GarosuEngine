#include <GarosuTypedef.h>

#include "GarosuThread.h"

#include <thread>
#include <chrono>
#include <atomic>

namespace Garosu
{

	static void DoWork(BaseWorker* worker)
	{
		worker->DoWork();
	}

	class BaseThread::impl
	{
	public:
		std::thread thr;
	};

	BaseThread::BaseThread(BaseWorker* worker)
		: mBaseWorker(worker)
		, pImpl(mk_uptr<impl>())
	{

	}

	BaseThread::~BaseThread(void)
	{

	}

	void BaseThread::Start(void)
	{
		pImpl->thr = std::thread(DoWork, mBaseWorker);
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
	class Looper : public BaseWorker
	{
	public:
		virtual void DoWork(void);

		BaseWorker* loopingTarget = nullptr;
		std::atomic<bool> NeedStop = false;
	};

	void Looper::DoWork(void)
	{
		while (true)
		{
			if (NeedStop) return;
			if (loopingTarget != nullptr) loopingTarget->DoWork();
		}
	}

	/*
	*
	*
	*
	*/
	class LoopThread::impl
	{
	public:
		impl(void);
		impl(const impl&) = delete;
		impl& operator=(const impl&) = delete;

		virtual ~impl(void);

		Looper mLooper;
	};

	LoopThread::impl::impl(void)
	{
	
	}

	LoopThread::impl::~impl(void)
	{

	}

	LoopThread::LoopThread(BaseWorker* onLoop)
		: pImpl(mk_uptr<impl>())
		, BaseThread(&pImpl->mLooper)
	{
		pImpl->mLooper.loopingTarget = onLoop;
	}

	LoopThread::~LoopThread(void)
	{

	}

	void LoopThread::Start(void)
	{
		pImpl->mLooper.NeedStop = false;
		BaseThread::Start();
	}

	void LoopThread::Stop(void)
	{
		pImpl->mLooper.NeedStop = true;
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