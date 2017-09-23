#include <memory>
#include <GarosuThread.h>

#include "GarosuScheduler.h"
#include "GarosuWorker.h"

namespace Garosu
{
	class Scheduler::impl
	{
	public:
		WorkerGroup mWorkerGroup;
	};

	Scheduler::Scheduler(void)
		: pImpl(std::make_unique<impl>())
	{

	}

	Scheduler::~Scheduler(void)
	{

	}

	bool Scheduler::Initialize(void)
	{

		return true;
	}

	bool Scheduler::HandoverTask(std::unique_ptr<BaseTask> newTask)
	{
		bool ret = pImpl->mWorkerGroup.Handover(newTask.get());

		return ret;
	}

}