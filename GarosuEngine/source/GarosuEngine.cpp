#include "GarosuEngine.h"

#include <memory>
#include <vector>
#include <functional>

#include <GarosuLog.h>
#include <GarosuTask.h>
#include <GarosuPhysics.h>
#include <GarosuGraphics.h>
#include <GarosuScheduler.h>

namespace Garosu
{

	class Engine::impl
	{
	public:
		impl(void);
		virtual ~impl(void);

		Physics physics;
		Graphics graphics;
		Scheduler scheduler;
	};

	Engine::impl::impl(void) {}
	Engine::impl::~impl(void) {}

	Engine::Engine(void)
		: pImpl(std::make_unique<impl>())
	{

	}

	Engine::~Engine(void)
	{

	}

	bool Engine::Initialize(void* outputTarget)
	{

		return true;
	}

	bool Engine::SendMessage(BaseEngineMessage* message)
	{

		return true;
	}

}