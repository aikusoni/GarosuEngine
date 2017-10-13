#include <GarosuTypedef.h>

#include "GarosuEngine.h"

// common
#include <GarosuTask.h>
#include <GarosuIScheduler.h>
#include <GarosuIPhysics.h>
#include <GarosuIGraphics.h>

// Garosu Common
#include <GarosuSettings.h>

// Garosu Utils
#include <GarosuLog.h>
#include <GarosuThread.h>

// garosu libraries
#include <GarosuPhysics.h>
#include <GarosuGraphics.h>
#include <GarosuScheduler.h>

namespace Garosu
{

	class Engine final : public IEngine
	{
	public:
		Engine(void);
		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;

		virtual ~Engine(void);

		virtual bool Initialize(void);
		virtual bool Finalize(void);

		virtual bool SendMessage(BaseMessage*);

	private:
		IScheduler* scheduler;
		IPhysics* physics;
		IGraphics* graphics;
	};

	Engine::Engine(void)
	{
		scheduler = nullptr;
		physics = nullptr;
		graphics = nullptr;
	}

	Engine::~Engine(void)
	{

	}

	bool Engine::Initialize(void)
	{
		///// Start Log Thread
		SETLOGLEVEL_DEBUG();
		//SETLOGLEVEL_INFO();
		LOGSTART();

		LOGQD("Garosu Engine initializing");

		bool initSuccess = false;
		do
		{
			///// Make Scheduler
			u32 numSchedulerThreads = ThreadUtils::GetConcurrencyCount();
			numSchedulerThreads -= 2; // for main thread & window thread

			auto scherr = Garosu::SchedulerFactory::MakeDefaultScheduler(&scheduler, numSchedulerThreads);
			if (scheduler == nullptr)
			{
				LOGQC("Scheduler allocation failed.");
				break;
			}

			if (scherr != SchedulerError::OK)
			{
				LOGC << "MakeDefaultSceduler() failed (err:" << (int)scherr << ")";
				break;
			}

			if (scheduler->Initialize() != SchedulerError::OK)
			{
				LOGQE("Scheduler initialization failed.");
				break;
			}

			LOGQN("Scheduler initialization success.");

			///// Make Physics
			physics = Garosu::PhysicsFactory::MakeDefaultPhysics(scheduler);
			if (physics == nullptr)
			{
				LOGQC("Physics allocation failed.");
				break;
			}

			if (physics->Initialize() != PhysicsError::OK)
			{
				LOGQE("Physics initialization failed.");
				break;
			}

			LOGQN("Physics initialization success.");

			///// Make Graphics
			graphics = Garosu::GraphicsFactory::MakeDefaultGraphics(scheduler);
			if (graphics == nullptr)
			{
				LOGQC("Graphics allocation failed.");
				break;
			}

			if (graphics->Initialize() != GraphicsError::OK)
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
		if (graphics)
		{
			graphics->Finalize();
			delete graphics;
			graphics = nullptr;
		}

		if (physics)
		{
			physics->Finalize();
			delete physics;
			physics = nullptr;
		}

		if (scheduler)
		{
			scheduler->Finalize();
			delete scheduler;
			scheduler = nullptr;
		}

		LOGSTOP();

		return true;
	}

	bool Engine::SendMessage(BaseMessage* message)
	{
		switch (message->mMessageId)
		{
		case EngineMessageId::Default:
			return true;

		case EngineMessageId::SetApplicationStoragePath:
		{
			auto& appPath = static_cast<StringMessage*>(message)->mStr;
			Settings::SetAppPath(appPath);
			return true;
		}

		}

		return false;
	}

}

__declspec(dllexport) Garosu::IEngine* MakeGarosuEngine(void)
{
	return new Garosu::Engine();
}

__declspec(dllexport) bool DeleteGarosuEngine(Garosu::IEngine* engine)
{
	if (engine != nullptr) {
		delete engine;
		return true;
	}

	return false;
}

__declspec(dllexport) bool Initialize(Garosu::IEngine* engine)
{
	return engine->Initialize();
}

__declspec(dllexport) bool Finalize(Garosu::IEngine* engine)
{
	return engine->Finalize();
}

__declspec(dllexport) bool SendMessage(Garosu::IEngine* engine, Garosu::BaseMessage* message)
{
	return engine->SendMessage(message);
}