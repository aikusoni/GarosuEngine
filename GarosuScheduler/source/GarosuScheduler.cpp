#include <memory>
#include <GarosuThread.h>

#include "GarosuScheduler.h"

namespace Garosu
{
	class Scheduler::impl
	{
	public:
		
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

	bool Scheduler::PutTask(std::unique_ptr<BaseWorker> newTask)
	{

		return true;
	}

}