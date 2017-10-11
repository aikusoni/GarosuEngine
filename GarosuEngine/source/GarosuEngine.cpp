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
		///// Start Log Thread
		SETLOGLEVELD();
		LOGSTART();

		LOGQD("Garosu Engine initializing");

		bool initSuccess = false;
		do
		{
			///// Make Scheduler
			u32 numSchedulerThreads = ThreadUtils::GetConcurrencyCount();
			numSchedulerThreads -= 2; // for main thread & window thread

			auto scherr = Garosu::SchedulerFactory::MakeDefaultScheduler(&pImpl->scheduler, numSchedulerThreads);
			if (pImpl->scheduler == nullptr)
			{
				LOGQC("Scheduler allocation failed.");
				break;
			}

			if (scherr != SchedulerError::OK)
			{
				LOGC << "MakeDefaultSceduler() failed (err:" << (int)scherr << ")";
				break;
			}

			if (pImpl->scheduler->Initialize() != SchedulerError::OK)
			{
				LOGQE("Scheduler initialization failed.");
				break;
			}

			LOGQN("Scheduler initialization success.");

			///// Make Physics
			pImpl->physics = Garosu::PhysicsFactory::MakeDefaultPhysics(pImpl->scheduler);
			if (pImpl->physics == nullptr)
			{
				LOGQC("Physics allocation failed.");
				break;
			}

			if (pImpl->physics->Initialize() != PhysicsError::OK)
			{
				LOGQE("Physics initialization failed.");
				break;
			}

			LOGQN("Physics initialization success.");

			///// Make Graphics
			pImpl->graphics = Garosu::GraphicsFactory::MakeDefaultGraphics(pImpl->scheduler);
			if (pImpl->graphics == nullptr)
			{
				LOGQC("Graphics allocation failed.");
				break;
			}

			if (pImpl->graphics->Initialize() != GraphicsError::OK)
			{
				LOGQE("Graphics initialization failed.");
				break;
			}

			LOGQN("Graphics initialization success.");

			///// Make Audio

			initSuccess = true;
		} while (false);

		if (!initSuccess)
		{
			LOGQE("Garosu Engine initialization failed.");
			return false;
		}

		LOGQD("Garosu Enigne initialization Success.");

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
		}

		LOGSTOP();

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