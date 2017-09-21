#include <memory>
#include <GarosuThread.h>

#include "GarosuScheduler.h"
#include "GarosuWorker.h"

#include <GarosuSignal.h>

namespace Garosu
{
	class Worker::impl
	{
	public:
		impl(void) {}
		virtual ~impl(void) {}

		bool mIsWorking;
	};

	Worker::Worker(void)
		: pImpl(std::make_unique<impl>())
	{
		pImpl->mIsWorking = false;
	}

	Worker::~Worker(void)
	{

	}

	void Worker::working(void)
	{
		while (pImpl->mIsWorking)
		{
			// do work
		}
	}

	class WorkerPool::impl
	{
	public:
		int mNum;
	};

	WorkerPool::WorkerPool(int num)
		: pImpl(std::make_unique<impl>())
	{
		pImpl->mNum = num;
	}

	WorkerPool::~WorkerPool(void)
	{

	}

	bool WorkerPool::Initialize(void)
	{

		return true;
	}

}