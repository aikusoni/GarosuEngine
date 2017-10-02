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
		pImpl->physics = NULL;
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
			///// Make Scheduler
			u32 numSchedulerThreads = ThreadUtils::GetConcurrencyCount();
			numSchedulerThreads -= 2; // for main thread & logger

			pImpl->scheduler = Garosu::SchedulerFactory::MakeDefaultScheduler(numSchedulerThreads);
			if (pImpl->scheduler == NULL)
			{
				LOGC("Scheduler allocation failed.");
				break;
			}

			if (pImpl->scheduler->Initialize() != SchedulerError::OK)
			{
				LOGE("Scheduler initialization failed.");
				break;
			}

			///// Make Physics
			pImpl->physics = Garosu::PhysicsFactory::MakeDefaultPhysics(pImpl->scheduler);
			if (pImpl->physics == NULL)
			{
				LOGC("Physics allocation failed.");
				break;
			}

			if (pImpl->physics->Initialize() != PhysicsError::OK)
			{
				LOGE("Physics initialization failed.");
				break;
			}

			///// Make Graphics
			pImpl->graphics = Garosu::GraphicsFactory::MakeDefaultGraphics(pImpl->scheduler);
			if (pImpl->graphics == NULL)
			{
				LOGC("Graphics allocation failed.");
				break;
			}

			if (pImpl->graphics->Initialize() != GraphicsError::OK)
			{
				LOGE("Graphics initialization failed.");
				break;
			}

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

	bool Engine::Finalize(void)
	{
		if (pImpl)
		{
			if (pImpl->graphics)
			{
				pImpl->graphics->Finalize();
				delete pImpl->graphics;
				pImpl->graphics = NULL;
			}

			if (pImpl->physics)
			{
				pImpl->physics->Finalize();
				delete pImpl->physics;
				pImpl->physics = NULL;
			}

			if (pImpl->scheduler)
			{
				pImpl->scheduler->Finalize();
				delete pImpl->scheduler;
				pImpl->scheduler = NULL;
			}
		}

		return true;
	}

	bool Engine::SendMessage(BaseEngineMessage* message)
	{

		return true;
	}

}