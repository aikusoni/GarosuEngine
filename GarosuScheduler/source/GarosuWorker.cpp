#include <memory>
#include <vector>
#include <atomic>
#include <utility>
#include <functional>

#include "GarosuTask.h"
#include "GarosuScheduler.h"
#include "GarosuWorker.h"

#include <concurrent_queue.h>

#include <GarosuThread.h>
#include <GarosuLockUtils.h>

namespace Garosu
{
	using TaskQueue = Concurrency::concurrent_queue<BaseTask*>;
	
	/*
	* TaskWorker executes tasks taked from task queue.
	* One TaskWorker runs a task at a time.
	*
	*/
	class TaskWorker final : public BaseWorker
	{
	public:
		TaskWorker(TaskQueue&);
		TaskWorker(const TaskWorker&) = delete;
		TaskWorker& operator=(const TaskWorker&) = delete;

		~TaskWorker(void);

		virtual void DoWork(void) override;

		std::atomic<bool> mLoop;
		std::atomic<bool> mDone;
		TaskQueue& mTaskQueue;
	};

	TaskWorker::TaskWorker(TaskQueue& taskQueue)
		: mLoop(false)
		, mDone(true)
		, mTaskQueue(taskQueue)
	{
		
	}

	TaskWorker::~TaskWorker(void)
	{

	}

	void TaskWorker::DoWork(void)
	{
		int tryCnt = 0;
		mDone = false;
		mLoop = true;
		while (mLoop)
		{
			BaseTask* task;
			if (!mTaskQueue.try_pop(task))
			{
				tryCnt++;
				if (tryCnt > 10000)
				{
					tryCnt = 0;
					ThreadUtils::SleepFor(10 * 1000 * 1000); // 10 ms
				}
				continue;
			}

			if (task == NULL) continue;

			// do task
			task->DoTask();
		}

		mDone = true;
	}

	/*
	* WorkerThread is a thread class for running TaskWorker.
	* 
	*
	*/
	class WorkerThread final : public BaseThread
	{
	public:
		WorkerThread(TaskQueue&);
		~WorkerThread(void);

		void RequestStop(void);
		bool IsDone(void);

	private:
		TaskWorker mWorker;
	};

	WorkerThread::WorkerThread(TaskQueue& taskQueue)
		: mWorker(taskQueue)
		, BaseThread(mWorker)
	{

	}

	WorkerThread::~WorkerThread(void)
	{

	}

	void WorkerThread::RequestStop(void)
	{
		mWorker.mLoop = false;
	}

	bool WorkerThread::IsDone(void)
	{
		return mWorker.mDone;
	}

	/*
	* WorkerGorup's pimpl idiom
	* WorkerGroup manages WorkerThread.
	*
	*/
	class WorkerGroup::impl
	{
	public:
		int mNumWorker;
		bool mIsInit;
		bool mStart;

		TaskQueue mTaskQueue;
		std::vector<std::unique_ptr<WorkerThread>> mWorkerThreads;
		std::vector<std::unique_ptr<WorkerThread>> mReducedWorkerThreads;
	};

	WorkerGroup::WorkerGroup(void)
		: pImpl(std::make_unique<impl>())
	{
		pImpl->mNumWorker = ThreadUtils::GetConcurrencyCount();
		pImpl->mIsInit = false;
		pImpl->mStart = false;
	}

	WorkerGroup::WorkerGroup(int numWorker)
		: pImpl(std::make_unique<impl>())
	{
		pImpl->mNumWorker = numWorker;
		pImpl->mIsInit = false;
	}

	WorkerGroup::~WorkerGroup(void)
	{
		Stop();
		for (auto& e : pImpl->mWorkerThreads)
			e->Join();

		for (auto& e : pImpl->mReducedWorkerThreads)
			e->Join();
	}

	bool WorkerGroup::Initialize(void)
	{
		if (!pImpl->mIsInit)
		{
			pImpl->mIsInit = true;
		
			for (int i = 0; i < pImpl->mNumWorker; ++i)
				pImpl->mWorkerThreads.push_back(std::make_unique<WorkerThread>(pImpl->mTaskQueue));
		}

		return true;
	}

	bool WorkerGroup::SetNumWorker(int numWorker)
	{
		if (!pImpl->mIsInit) {
			pImpl->mNumWorker = numWorker;
			return true;
		}

		if (numWorker > pImpl->mNumWorker)
		{
			int numExpand = numWorker - pImpl->mNumWorker;

			for (int i = 0; i < numExpand; ++i)
				pImpl->mWorkerThreads.push_back(std::make_unique<WorkerThread>(pImpl->mTaskQueue));

			pImpl->mNumWorker = numWorker;

			if (pImpl->mStart)
			{
				for (int i = pImpl->mNumWorker - numExpand; i < pImpl->mNumWorker; ++i)
				{
					pImpl->mReducedWorkerThreads[i]->Start();
				}
			}
		}
		else if (numWorker < pImpl->mNumWorker)
		{
			int numReduce = pImpl->mNumWorker;

			for (int i = 0; i < numReduce; ++i)
			{
				auto reducing = std::move(pImpl->mWorkerThreads[pImpl->mWorkerThreads.size() - 1]);
				reducing->RequestStop();
				pImpl->mReducedWorkerThreads.push_back(std::move(reducing));
			}

			pImpl->mNumWorker = numWorker;

			for (size_t i = 0; i < pImpl->mReducedWorkerThreads.size(); ++i)
				pImpl->mReducedWorkerThreads[i]->Join();

			pImpl->mReducedWorkerThreads.clear();
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

		return true;
	}

	bool WorkerGroup::Stop(void)
	{
		if (!pImpl->mStart) return false;

		for (auto& e : pImpl->mWorkerThreads)
			e->RequestStop();

		return true;
	}

	bool WorkerGroup::Handover(BaseTask* task)
	{
		pImpl->mTaskQueue.push(task);

		return true;
	}

}