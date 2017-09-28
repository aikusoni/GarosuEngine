#include "GarosuEngine.h"

#include <memory>
#include <vector>
#include <functional>

#include <SchedulerInterface.h>

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

		//Scheduler scheduler;
		//Physics physics;
		//Graphics graphics;
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
		LOGD("Garosu Engine initializing");

		bool initFailed = true;
		do
		{
			//if (pImpl->scheduler.Initialize() != SchedulerError::OK)
			//{
			//	LOGE("cannot intialize scheduler library");
			//	break;
			//}
			//LOGD("scheduler library initialized...");

			//if (pImpl->physics.Initialize() != PhysicsError::OK)
			//{
			//	LOGE("cannot initialize physics library");
			//	break;
			//}
			//LOGD("physics library initialized...");

			//if (pImpl->graphics.Initialize() != GraphicsError::OK)
			//{
			//	LOGE("cannot initialize graphics library");
			//	break;
			//}
			//LOGD("graphics library initialized...");

			initFailed = false;

		} while (false);

		if (initFailed)
		{
			LOGE("cannot initialize garosu engine");
			return false;
		}

		LOGD("Garosu Enigne initializing completed.");

		return true;
	}

	bool Engine::SendMessage(BaseEngineMessage* message)
	{

		return true;
	}

}