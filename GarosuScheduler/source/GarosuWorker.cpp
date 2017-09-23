#include <memory>

#include "GarosuWorker.h"

#include <vector>
#include <atomic>

#include <GarosuThread.h>
#include <GarosuSignal.h>

namespace Garosu
{
	
	class TaskWorker final : public BaseWorker
	{
	public:
		TaskWorker(void);
		TaskWorker(const TaskWorker&) = delete;
		TaskWorker& operator=(const TaskWorker&) = delete;

		~TaskWorker(void);

		virtual void DoWork(void) override;

		std::atomic<bool> mLoop;
	};

	TaskWorker::TaskWorker(void)
		: mLoop(false)
	{
		
	}

	TaskWorker::~TaskWorker(void)
	{

	}

	void TaskWorker::DoWork(void)
	{
		mLoop = true;
		while (mLoop)
		{

		}
	}

	class WorkerThread final : public BaseThread
	{
	public:
		WorkerThread(void);
		~WorkerThread(void);

		virtual void RequestStop(void);

	private:
		TaskWorker worker;
	};

	WorkerThread::WorkerThread(void)
		: BaseThread(worker)
	{

	}

	WorkerThread::~WorkerThread(void)
	{
		// join here?
	}

	void WorkerThread::RequestStop(void)
	{
		worker.mLoop = false;
	}

	class WorkerGroup::impl
	{
	public:
		int mNumWorker;
		bool mIsInit;

		std::vector<std::unique_ptr<WorkerThread>> mWorkerThreads;
	};

	WorkerGroup::WorkerGroup(int numWorker)
		: pImpl(std::make_unique<impl>())
	{
		pImpl->mNumWorker = numWorker;
		pImpl->mIsInit = false;
	}

	WorkerGroup::~WorkerGroup(void)
	{
		for (auto& e : pImpl->mWorkerThreads)
			e->Join();
	}

	bool WorkerGroup::Initialize(void)
	{
		if (!pImpl->mIsInit)
		{
			pImpl->mIsInit = true;
		
			for (int i = 0; i < pImpl->mNumWorker; ++i)
				pImpl->mWorkerThreads.push_back(std::make_unique<WorkerThread>());
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

}