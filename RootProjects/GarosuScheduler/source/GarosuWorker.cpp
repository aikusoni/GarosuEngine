#include <GarosuTypedef.h>
#include <GarosuTask.h>

#include <vector>
#include <atomic>
#include <utility>
#include <functional>

#include "GarosuWorker.h"

#include <GarosuThread.h>
#include <GarosuLockUtils.h>
#include <GarosuMathUtils.h>

namespace Garosu
{

	inline u32 SaturateConcurrency(u32 n) {
		return MathUtils::Clamp<u32>(n, 1u, ThreadUtils::GetConcurrencyCount());
	}

	/*
	* WorkerThread is a thread class for running TaskWorker.
	* 
	*
	*/
	class WorkerThread final : public LoopThread
	{
	public:
		WorkerThread(ITaskProvider*);
		~WorkerThread(void);

		virtual bool OnLoop(void);

	private:
		u32 mTryCnt = 0;
		ITaskProvider* mTaskProvider;
		Locker mLocker;
	};

	WorkerThread::WorkerThread(ITaskProvider* taskProvider)
		: mTaskProvider(taskProvider)
	{
		mTryCnt = 0;
	}

	WorkerThread::~WorkerThread(void)
	{

	}

	bool WorkerThread::OnLoop(void)
	{
		BaseTask* task = nullptr;
		task = mTaskProvider->GetTask();
		if (task == nullptr) {
			mTryCnt++;
			if (mTryCnt > 10000u)
			{
				mTryCnt = 0;
				ThreadUtils::SleepFor(1000u); // 1 ms
			}
			return true;
		}

		// do task
		task->DoTask(); // TODO catch exception & save it.

		return true;
	}

	/*
	* WorkerGorup's pimpl idiom
	* WorkerGroup manages WorkerThread.
	*
	*/
	class WorkerGroup::impl
	{
	public:
		u32 mNumWorker;
		bool mIsInit;
		bool mStart;

		ITaskProvider* mTaskProvider;

		std::vector<uptr<WorkerThread>> mWorkerThreads;
	};

	WorkerGroup::WorkerGroup(u32 numWorker, ITaskProvider* taskProvider)
		: pImpl(mk_uptr<impl>())
	{
		pImpl->mNumWorker = SaturateConcurrency(numWorker);
		pImpl->mIsInit = false;
		pImpl->mStart = false;
		pImpl->mTaskProvider = taskProvider;
	}

	WorkerGroup::~WorkerGroup(void)
	{
		Stop();
		Join();
	}

	bool WorkerGroup::Initialize(void)
	{
		if (!pImpl->mIsInit)
		{
			pImpl->mIsInit = true;
		
			for (u32 i = 0; i < pImpl->mNumWorker; ++i)
				pImpl->mWorkerThreads.push_back(mk_uptr<WorkerThread>(pImpl->mTaskProvider));
		}

		return true;
	}

	bool WorkerGroup::SetNumWorker(u32 numWorker)
	{
		SaturateConcurrency(numWorker);

		if (!pImpl->mIsInit) {
			pImpl->mNumWorker = numWorker;
			return true;
		}

		if (numWorker > pImpl->mNumWorker)
		{
			u32 numExpand = numWorker - pImpl->mNumWorker;

			for (u32 i = 0; i < numExpand; ++i)
				pImpl->mWorkerThreads.push_back(mk_uptr<WorkerThread>(pImpl->mTaskProvider));

			pImpl->mNumWorker = numWorker;

			if (pImpl->mStart)
			{
				for (u32 i = pImpl->mNumWorker - numExpand; i < pImpl->mNumWorker; ++i)
				{
					pImpl->mWorkerThreads[i]->Start();
				}
			}
		}
		else if (numWorker < pImpl->mNumWorker)
		{
			i32 numReduce = pImpl->mNumWorker;

			std::vector<uptr<WorkerThread>> reducedWorkerThreads;

			for (i32 i = 0; i < numReduce; ++i)
			{
				auto reducing = std::move(pImpl->mWorkerThreads.back());
				pImpl->mWorkerThreads.pop_back();
				reducing->Stop();
				reducedWorkerThreads.push_back(std::move(reducing));
			}

			pImpl->mNumWorker = numWorker;

			for (size_t i = 0; i < reducedWorkerThreads.size(); ++i)
				reducedWorkerThreads[i]->Join();

			reducedWorkerThreads.clear();
		}
		else
		{
			// don't change anything.
		}

		return true;
	}

	bool WorkerGroup::Start(void)
	{
		if (pImpl->mStart) return false;

		for (auto& e : pImpl->mWorkerThreads)
			e->Start();

		pImpl->mStart = true;

		return true;
	}

	bool WorkerGroup::Stop(void)
	{
		if (!pImpl->mStart) return false;

		for (auto& e : pImpl->mWorkerThreads)
			e->Stop();

		return true;
	}

	bool WorkerGroup::Join(void)
	{
		for (auto& e : pImpl->mWorkerThreads)
			e->Join();

		return true;
	}

}