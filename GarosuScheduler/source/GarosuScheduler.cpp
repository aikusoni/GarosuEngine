#include <GarosuTypedef.h>

#include <GarosuTask.h>
#include <GarosuIScheduler.h>

#include "GarosuScheduler.h"
#include "GarosuWorker.h"

#include <GarosuLog.h>

namespace Garosu
{

	class Scheduler : public IScheduler
	{
	public:
		Scheduler(u32 numThread);
		Scheduler(const Scheduler&) = delete;
		Scheduler& operator=(const Scheduler&) = delete;

		virtual ~Scheduler(void);

		virtual SchedulerError Initialize(void);
		virtual SchedulerError Finalize(void);

		virtual SchedulerError HandoverTask(uptr<BaseTask> newTask);

		WorkerGroup mWorkerGroup;
	};

	Scheduler::Scheduler(u32 numThread)
		: mWorkerGroup(numThread)
	{

	}

	Scheduler::~Scheduler(void)
	{
		mWorkerGroup.Stop();
	}

	SchedulerError Scheduler::Initialize(void)
	{
		if (!mWorkerGroup.Initialize())
		{
			LOGE("[Scheduler] cannot intialize WorkerGroup");
			return SchedulerError::ERROR;
		}

		if (!mWorkerGroup.Start())
		{
			LOGE("[Scheduler] cannot start WorkerGroup");
			return SchedulerError::ERROR;
		}

		return SchedulerError::OK;
	}

	SchedulerError Scheduler::Finalize(void)
	{
		bool ret = mWorkerGroup.Stop();

		return ret ? SchedulerError::OK : SchedulerError::ERROR;
	}

	SchedulerError Scheduler::HandoverTask(uptr<BaseTask> newTask)
	{
		bool ret = mWorkerGroup.Handover(newTask.get());

		return ret ? SchedulerError::OK : SchedulerError::ERROR;
	}


	IScheduler* SchedulerFactory::MakeDefaultScheduler(u32 numThread)
	{
		return new Scheduler(numThread);
	}

}