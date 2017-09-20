#include <memory>

#include "Thread.h"

#include <thread>
#include <chrono>

static void DoWork(BaseWorker* worker)
{
	worker->DoWork();
}

class BaseThread::impl
{
public:
	std::thread thr;
};

BaseThread::BaseThread(BaseWorker& baseWorker)
	: mBaseWorker(baseWorker)
	, pImpl(std::make_unique<impl>())
{

}

BaseThread::~BaseThread(void)
{

}

void BaseThread::Start(void)
{
	pImpl->thr = std::thread(DoWork, &mBaseWorker);
}

void BaseThread::Join(void)
{
	if (pImpl->thr.joinable())
		pImpl->thr.join();
}

void ThreadUtils::SleepFor(int nanoSeconds)
{
	std::this_thread::sleep_for(std::chrono::nanoseconds(nanoSeconds));
}