#include <GarosuTypedef.h>

#include "GarosuThread.h"

#include <thread>
#include <chrono>

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

	BaseThread::BaseThread(BaseWorker* baseWorker)
		: mBaseWorker(baseWorker)
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

	void ThreadUtils::SleepFor(i32 nanoSeconds)
	{
		std::this_thread::sleep_for(std::chrono::nanoseconds(nanoSeconds));
	}

	u32 ThreadUtils::GetConcurrencyCount(void)
	{
		return std::thread::hardware_concurrency();
	}

}