#include "GarosuEngine.h"

#include <memory>
#include <functional>

#include <GarosuTask.h>
#include <GarosuPhysics.h>
#include <GarosuGraphics.h>
#include <GarosuScheduler.h>

namespace Garosu
{

	class Engine::impl
	{
	public:
	};

	Engine::Engine(void)
		: pImpl(std::make_unique<impl>())
	{

	}

	Engine::~Engine(void)
	{

	}

	bool Engine::SendMessage(BaseEngineMessage* message)
	{

		return true;
	}

}