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
		pImpl->scheduler = nullptr;
		pImpl->physics = nullptr;
		pImpl->graphics = nullptr;
	}

	Engine::~Engine(void)
	{

	}

	bool Engine::Initialize(void)
	{
		LOGD("Garosu Engine initializing");

		bool initSuccess = false;
		do
		{
			///// Start Log Thread
			SETLOGLEVELD();
			LOGSTART();

			///// Make Scheduler
			u32 numSchedulerThreads = ThreadUtils::GetConcurrencyCount();
			numSchedulerThreads -= 2; // for main thread & window thread

			auto scherr = Garosu::SchedulerFactory::MakeDefaultScheduler(&pImpl->scheduler, numSchedulerThreads);
			if (pImpl->scheduler == nullptr)
			{
				LOGC("Scheduler allocation failed.");
				break;
			}
			if (scherr != SchedulerError::OK)
			{
				LOGC("MakeDefaultScheduler() failed.("    ")");
				break;
			}

			if (pImpl->scheduler->Initialize() != SchedulerError::OK)
			{
				LOGE("Scheduler initialization failed.");
				break;
			}

			///// Make Physics
			pImpl->physics = Garosu::PhysicsFactory::MakeDefaultPhysics(pImpl->scheduler);
			if (pImpl->physics == nullptr)
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
			if (pImpl->graphics == nullptr)
			{
				LOGC("Graphics allocation failed.");
				break;
			}

			if (pImpl->graphics->Initialize() != GraphicsError::OK)
			{
				LOGE("Graphics initialization failed.");
				break;
			}

			///// Make Audio

			initSuccess = true;
		} while (false);

		if (!initSuccess)
		{
			LOGE("Garosu Engine initialization failed.");
			return false;
		}

		LOGD("Garosu Enigne initialization Success.");

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
				pImpl->graphics = nullptr;
			}

			if (pImpl->physics)
			{
				pImpl->physics->Finalize();
				delete pImpl->physics;
				pImpl->physics = nullptr;
			}

			if (pImpl->scheduler)
			{
				pImpl->scheduler->Finalize();
				delete pImpl->scheduler;
				pImpl->scheduler = nullptr;
			}

			LOGSTOP();
		}

		return true;
	}

	bool Engine::SendMessage(BaseEngineMessage* message)
	{
		switch (message->mMessageId)
		{
		case EngineMessageId::Default:
			break;


		}

		return true;
	}

}