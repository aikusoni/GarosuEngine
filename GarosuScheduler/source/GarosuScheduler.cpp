#include <GarosuTypedef.h>

#include <GarosuTask.h>
#include <GarosuInterfaces.h>

#include "GarosuTask.h"
#include "GarosuScheduler.h"
#include "GarosuWorker.h"

#include <GarosuLog.h>


namespace Garosu
{

	class Scheduler::impl
	{
	public:
		impl(void) {}
		virtual ~impl(void) {}

		WorkerGroup mWorkerGroup;
	};

	Scheduler::Scheduler(void)
		: pImpl(mk_uptr<impl>())
	{

	}

	Scheduler::~Scheduler(void)
	{
		if (pImpl) pImpl->mWorkerGroup.Stop();
	}

	SchedulerError Scheduler::Initialize(void)
	{
		if (!pImpl->mWorkerGroup.Initialize())
		{
			LOGE("[Scheduler] cannot intialize WorkerGroup");
			return SchedulerError::ERROR;
		}

		if (!pImpl->mWorkerGroup.Start())
		{
			LOGE("[Scheduler] cannot start WorkerGroup");
			return SchedulerError::ERROR;
		}

		return SchedulerError::OK;
	}

	bool Scheduler::HandoverTask(uptr<BaseTask> newTask)
	{
		bool ret = pImpl->mWorkerGroup.Handover(newTask.get());

		return ret;
	}

}