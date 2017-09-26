#include <memory>

#include <functional>

#include "GarosuTask.h"
#include "GarosuScheduler.h"
#include "GarosuWorker.h"

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