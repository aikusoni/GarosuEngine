#include <memory>

#include "GarosuScheduler.h"
#include "GarosuWorker.h"

#include <vector>
#include <atomic>

#include <concurrent_queue.h>

#include <GarosuThread.h>
#include <GarosuSignal.h>

namespace Garosu
{
	using TaskQueue = Concurrency::concurrent_queue<BaseTask*>;
	
	class TaskWorker final : public BaseWorker
	{
	public:
		TaskWorker(TaskQueue&);
		TaskWorker(const TaskWorker&) = delete;
		TaskWorker& operator=(const TaskWorker&) = delete;

		~TaskWorker(void);

		virtual void DoWork(void) override;

		std::atomic<bool> mLoop;
		TaskQueue& mTaskQueue;
	};

	TaskWorker::TaskWorker(TaskQueue& taskQueue)
		: mLoop(false)
		, mTaskQueue(taskQueue)
	{
		
	}

	TaskWorker::~TaskWorker(void)
	{

	}

	void TaskWorker::DoWork(void)
	{
		int tryCnt = 0;
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
	}

	class WorkerThread final : public BaseThread
	{
	public:
		WorkerThread(TaskQueue&);
		~WorkerThread(void);

		virtual void RequestStop(void);

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

	class WorkerGroup::impl
	{
	public:
		int mNumWorker;
		bool mIsInit;

		std::vector<std::unique_ptr<WorkerThread>> mWorkerThreads;
		TaskQueue mTaskQueue;
	};

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

	bool WorkerGroup::Start(void)
	{
		for (auto& e : pImpl->mWorkerThreads)
			e->Start();

		return true;
	}

	bool WorkerGroup::Stop(void)
	{
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