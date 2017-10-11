#include <GarosuTypedef.h>

#include <GarosuTask.h>
#include <GarosuIScheduler.h>

#include "GarosuScheduler.h"
#include "GarosuWorker.h"

#include <GarosuLog.h>

#include <unordered_map>
#include <concurrent_queue.h>

namespace Garosu
{

	class ITaskManager : public ITaskProvider
	{
	public:
		virtual bool Handover(TaskSource, BaseTask*) = 0;
	};

	/*
	* Default Scheduler
	*
	* 
	*/
	class DefaultScheduler : public IScheduler
	{
	public:
		DefaultScheduler(uptr<ITaskManager>, u32);
		DefaultScheduler(const DefaultScheduler&) = delete;
		DefaultScheduler& operator=(const DefaultScheduler&) = delete;

		virtual ~DefaultScheduler(void);

		virtual SchedulerError Initialize(void);
		virtual SchedulerError Finalize(void);

		virtual SchedulerError HandoverTask(TaskSource, BaseTask*);

		uptr<ITaskManager> mTaskManager;
		WorkerGroup mWorkerGroup;
	};

	DefaultScheduler::DefaultScheduler(uptr<ITaskManager> taskMgr, u32 numThread)
		: mTaskManager(std::move(taskMgr))
		, mWorkerGroup(numThread, mTaskManager.get())
	{
		
	}

	DefaultScheduler::~DefaultScheduler(void)
	{
		mWorkerGroup.Stop();
	}

	SchedulerError DefaultScheduler::Initialize(void)
	{
		if (!mWorkerGroup.Initialize())
		{
			LOGQE("[Scheduler] cannot intialize WorkerGroup");
			return SchedulerError::ERROR;
		}

		if (!mWorkerGroup.Start())
		{
			LOGQE("[Scheduler] cannot start WorkerGroup");
			return SchedulerError::ERROR;
		}

		return SchedulerError::OK;
	}

	SchedulerError DefaultScheduler::Finalize(void)
	{
		bool ret = mWorkerGroup.Stop();

		return ret ? SchedulerError::OK : SchedulerError::ERROR;
	}

	SchedulerError DefaultScheduler::HandoverTask(TaskSource taskSource, BaseTask* newTask)
	{
		bool ret = mTaskManager->Handover(taskSource, newTask);

		return ret ? SchedulerError::OK : SchedulerError::ERROR;
	}


	/*
	*
	*
	*
	*/
	class FIFOTaskManager : public ITaskManager
	{
		using TaskQueue = Concurrency::concurrent_queue<BaseTask*>; // lock-free queue

	public:
		virtual BaseTask* GetTask(void);
		virtual bool Handover(TaskSource, BaseTask*);

		TaskQueue mQueue;
	};

	BaseTask* FIFOTaskManager::GetTask(void)
	{
		BaseTask* task;
		if (mQueue.try_pop(task)) return task;
		return nullptr;
	}

	bool FIFOTaskManager::Handover(TaskSource taskSource, BaseTask* newTask)
	{
		// FIFOTaskManager does not consider taskSource;
		mQueue.push(newTask);

		return true;
	}

	/*
	*
	*
	*
	*/
	class BalancingTaskManager : public ITaskManager
	{
		using TaskQueue = Concurrency::concurrent_queue<BaseTask*>; // lock-free queue

	public:
		virtual BaseTask* GetTask(void);
		virtual bool Handover(TaskSource, BaseTask*);

		std::unordered_map<TaskSource, TaskQueue> mQueues;
		TaskQueue* nextTaskQueue;
	};

	BaseTask* BalancingTaskManager::GetTask(void)
	{
		// TODO

		return nullptr;
	}

	bool BalancingTaskManager::Handover(TaskSource taskSource, BaseTask* newTask)
	{
		mQueues[taskSource].push(newTask);

		return true;
	}

	/*
	* SchedulerFactory
	*/
	SchedulerError SchedulerFactory::MakeDefaultScheduler(IScheduler** scheduler, u32 numThread)
	{
		if (scheduler == nullptr) return SchedulerError::ERROR;
		if (*scheduler != nullptr) return SchedulerError::ERROR;

		try {
			*scheduler = new DefaultScheduler(mk_uptr<FIFOTaskManager>(), numThread);
		}
		catch (std::bad_alloc ba)
		{
			return SchedulerError::ERROR_MEM_ALLOC_FAILED;
		}

		return SchedulerError::OK;
	}

	SchedulerError SchedulerFactory::MakeBalancedScheduler(IScheduler** scheduler, u32 numThread)
	{
		if (scheduler == nullptr) return SchedulerError::ERROR;
		if (*scheduler != nullptr) return SchedulerError::ERROR;

		try
		{
			*scheduler = new DefaultScheduler(mk_uptr<BalancingTaskManager>(), numThread);
		}
		catch (std::bad_alloc ba)
		{
			return SchedulerError::ERROR_MEM_ALLOC_FAILED;
		}

		return SchedulerError::OK;
	}

}