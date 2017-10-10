#include <GarosuTypedef.h>

#include <GarosuTask.h>
#include <GarosuIScheduler.h>

#include "GarosuScheduler.h"
#include "GarosuWorker.h"

#include <GarosuLog.h>

namespace Garosu
{
	/*
	* Default Scheduler
	*
	* FIFO Scheduler
	*/
	class DefaultScheduler : public IScheduler
	{
	public:
		DefaultScheduler(u32 numThread);
		DefaultScheduler(const DefaultScheduler&) = delete;
		DefaultScheduler& operator=(const DefaultScheduler&) = delete;

		virtual ~DefaultScheduler(void);

		virtual SchedulerError Initialize(void);
		virtual SchedulerError Finalize(void);

		virtual SchedulerError HandoverTask(TaskSource, BaseTask*);

		WorkerGroup mWorkerGroup;

		class DefaultTaskProvider : public ITaskProvider
		{
		public:
			virtual BaseTask* GetTask(void)
			{
				return nullptr;	// TODO
			}
		} mTaskProvider;
	};

	DefaultScheduler::DefaultScheduler(u32 numThread)
		: mWorkerGroup(numThread, &mTaskProvider)
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
		bool ret = mWorkerGroup.Handover(newTask);

		return ret ? SchedulerError::OK : SchedulerError::ERROR;
	}

	
	/* BalancedScheduler
	*
	*
	* 
	*/
	class BalancedScheduler : public IScheduler
	{
	public:
		BalancedScheduler(u32 numThread);
		BalancedScheduler(const BalancedScheduler&) = delete;
		BalancedScheduler& operator=(const BalancedScheduler&) = delete;

		virtual ~BalancedScheduler(void);

		virtual SchedulerError Initialize(void);
		virtual SchedulerError Finalize(void);

		virtual SchedulerError HandoverTask(TaskSource, BaseTask*);

		WorkerGroup mWorkerGroup;

		class BalancedTaskProvider : public ITaskProvider
		{
		public:
			virtual BaseTask* GetTask(void)
			{
				return nullptr;	// TODO
			}
		} mTaskProvider;
	};

	BalancedScheduler::BalancedScheduler(u32 numThread)
		: mWorkerGroup(numThread, &mTaskProvider)
	{

	}

	BalancedScheduler::~BalancedScheduler(void)
	{
		mWorkerGroup.Stop();
	}

	SchedulerError BalancedScheduler::Initialize(void)
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

	SchedulerError BalancedScheduler::Finalize(void)
	{
		bool ret = mWorkerGroup.Stop();

		return ret ? SchedulerError::OK : SchedulerError::ERROR;
	}

	SchedulerError BalancedScheduler::HandoverTask(TaskSource taskSource, BaseTask* newTask)
	{
		bool ret = mWorkerGroup.Handover(newTask);

		return ret ? SchedulerError::OK : SchedulerError::ERROR;
	}


	/*
	* SchedulerFactory
	*/
	SchedulerError SchedulerFactory::MakeDefaultScheduler(IScheduler** scheduler, u32 numThread)
	{
		if (scheduler == nullptr) return SchedulerError::ERROR;
		if (*scheduler != nullptr) return SchedulerError::ERROR;

		try {
			*scheduler = new DefaultScheduler(numThread);
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
			*scheduler = new BalancedScheduler(numThread);
		}
		catch (std::bad_alloc ba)
		{
			return SchedulerError::ERROR_MEM_ALLOC_FAILED;
		}

		return SchedulerError::OK;
	}

	//SchedulerError SchedulerFactory::DeleteScheduler(IScheduler** scheduler)
	//{
	//	if (scheduler == nullptr) return SchedulerError::ERROR;
	//	if (*scheduler == nullptr) return SchedulerError::ERROR;

	//	delete *scheduler;
	//	*scheduler = nullptr;

	//	return SchedulerError::OK;
	//}

}