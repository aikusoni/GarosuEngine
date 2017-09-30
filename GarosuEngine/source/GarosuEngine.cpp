#include <GarosuTypedef.h>

#include "GarosuEngine.h"

// common
#include <GarosuTask.h>
#include <GarosuIScheduler.h>
#include <GarosuIPhysics.h>
#include <GarosuIGraphics.h>

// Garosu Utils
#include <GarosuLog.h>
#include <GarosuThread.h>

// garosu libraries
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

		IScheduler* scheduler;
		IPhysics* physics;
		IGraphics* graphics;
	};

	Engine::impl::impl(void) {}
	Engine::impl::~impl(void) {}

	Engine::Engine(void)
		: pImpl(mk_uptr<impl>())
	{
		pImpl->scheduler = NULL;
		pImpl->graphics = NULL;
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
			int numSchedulerThreads = ThreadUtils::GetConcurrencyCount();
			numSchedulerThreads -= 1; // for main thread

			pImpl->scheduler = Garosu::SchedulerFactory::MakeDefaultScheduler(8);
			if (pImpl->scheduler == NULL)
			{
				// TODO
			}

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