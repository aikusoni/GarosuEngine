#include <GarosuTypedef.h>

#include "GarosuEngineAPI.h"

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

#include <GarosuNene.h>

namespace Garosu
{

	class ParameterContainer::impl
	{
	public:
		std::map<String, Nene> params;
	};

	ParameterContainer::ParameterContainer(void)
		: pImpl(mk_uptr<impl>())
	{

	}

	ParameterContainer::~ParameterContainer(void)
	{

	}

	bool ParameterContainer::SetParam(std::string paramName, bool paramValue)
	{
		pImpl->params[paramName] = paramValue;
		return true;
	}

	bool ParameterContainer::SetParam(std::string paramName, void* paramValue)
	{
		pImpl->params[paramName] = (i64)paramValue;
		return true;
	}

	bool ParameterContainer::SetParam(std::string paramName, long long paramValue)
	{
		pImpl->params[paramName] = paramValue;
		return true;
	}

	bool ParameterContainer::SetParam(std::string paramName, double paramValue)
	{
		pImpl->params[paramName] = paramValue;
		return true;
	}

	bool ParameterContainer::SetParam(std::string paramName, std::string paramValue)
	{
		pImpl->params[paramName] = paramValue;
		return true;
	}

	bool ParameterContainer::GetParam(std::string paramName, bool& paramValue)
	{
		auto& param = pImpl->params.find(paramName);
		if (param == pImpl->params.end()) return false;
		if ((*param).second.HasType<bool>() == false) return false;

		(*param).second.As<bool>(paramValue);

		return true;
	}

	bool ParameterContainer::GetParam(std::string paramName, void*& paramValue)
	{
		auto& param = pImpl->params.find(paramName);
		if (param == pImpl->params.end()) return false;
		if ((*param).second.HasType<i64>() == false) return false;

		(*param).second.As<i64>((i64&)paramValue);

		return true;
	}

	bool ParameterContainer::GetParam(std::string paramName, long long& paramValue)
	{
		auto& param = pImpl->params.find(paramName);
		if (param == pImpl->params.end()) return false;
		if ((*param).second.HasType<long long>() == false) return false;

		(*param).second.As<long long>(paramValue);

		return true;
	}

	bool ParameterContainer::GetParam(std::string paramName, double& paramValue)
	{
		auto& param = pImpl->params.find(paramName);
		if (param == pImpl->params.end()) return false;
		if ((*param).second.HasType<double>() == false) return false;

		(*param).second.As<double>(paramValue);

		return true;
	}

	bool ParameterContainer::GetParam(std::string paramName, std::string& paramValue)
	{
		auto& param = pImpl->params.find(paramName);
		if (param == pImpl->params.end()) return false;
		if ((*param).second.HasType<std::string>() == false) return false;

		(*param).second.As<std::string>(paramValue);

		return true;
	}

	BaseMessage::BaseMessage(EngineMessageId msgId)
		: mMsgId(msgId)
	{

	}

	BaseEvent::BaseEvent(EngineEventId evtId)
		: mEvtId(evtId)
	{

	}

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
		virtual bool RegisterCallback(EngineCallback*);

		bool CallCB(BaseEvent*);

	private:
		IScheduler* scheduler;
		IPhysics* physics;
		IGraphics* graphics;

		EngineCallback* callback;
	};

	Engine::Engine(void)
	{
		scheduler = nullptr;
		physics = nullptr;
		graphics = nullptr;
		callback = nullptr;
	}

	Engine::~Engine(void)
	{
	}

	bool Engine::Initialize(void)
	{
		///// Start Log Thread
		//SETLOGLEVEL_DEBUG();
		SETLOGLEVEL_INFO();
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
		switch (message->mMsgId)
		{
		case EngineMessageId::None:
			return true;

		case EngineMessageId::TestMessage:
		{
			bool boolParam = false;
			if (message->GetParam("bool", boolParam))
				LOGD << "TestMessage " << "bool param : " << boolParam;

			void* voidPtr = nullptr;
			if (message->GetParam("voidptr", voidPtr))
				LOGD << "TestMessage " << "voidptr param : " << voidPtr;

			i64 integerVal = 0;
			if (message->GetParam("integer", integerVal))
				LOGD << "TestMessage " << "voidptr param : " << integerVal;

			f64 doubleVal = 0.0;
			if (message->GetParam("double", doubleVal))
				LOGD << "TestMessage " << "double param : " << doubleVal;

			String strVal = "";
			if (message->GetParam("string", strVal))
				LOGD << "TestMessage " << "string param : " << strVal;

		}
			return true;

		case EngineMessageId::SetApplicationStoragePath:
		{
			String appPath;
			if (message->GetParam("AppPath", appPath) == false)
				return false;

			Settings::SetAppPath(appPath);
				
			return true;
		}

		}

		LOGFD("Engine::SendMessage has no routine to process this message(msg Id:%)", (u32)message->mMsgId);

		return false;
	}

	bool Engine::RegisterCallback(EngineCallback* clientCallback)
	{
		callback = clientCallback;
		return true;
	}

	bool Engine::CallCB(BaseEvent* eventInstance)
	{
		if (callback == nullptr) return false;
		else return callback(eventInstance);
	}

} // namespace Garosu

G_EXPORT Garosu::IEngine* CreateEngine(void)
{
	return new Garosu::Engine();
}

G_EXPORT bool DeleteEngine(Garosu::IEngine* engine)
{
	if (engine != nullptr) {
		delete engine;
		return true;
	}

	return false;
}

G_EXPORT bool Initialize(Garosu::IEngine* engine)
{
	return engine->Initialize();
}

G_EXPORT bool Finalize(Garosu::IEngine* engine)
{
	return engine->Finalize();
}

G_EXPORT bool SendMessage(Garosu::IEngine* engine, Garosu::BaseMessage* message)
{
	return engine->SendMessage(message);
}

G_EXPORT bool RegisterCallback(Garosu::IEngine* engine, Garosu::EngineCallback* callback)
{
	return engine->RegisterCallback(callback);
}

G_EXPORT Garosu::BaseMessage* CreateMessage(unsigned int msgId)
{
	return new Garosu::BaseMessage((Garosu::EngineMessageId)msgId);
}

G_EXPORT bool DeleteMessage(Garosu::BaseMessage* msg)
{
	if (msg != nullptr) {
		delete msg;
		return true;
	}

	return false;
}

G_EXPORT bool SetParam_Bool(Garosu::ParameterContainer* paramCont, char* paramName, bool paramValue)
{
	if (paramCont == nullptr) return false;
	return paramCont->SetParam(paramName, paramValue);
}

G_EXPORT bool SetParam_VoidPtr(Garosu::ParameterContainer* paramCont, char* paramName, void* paramValue)
{
	if (paramCont == nullptr) return false;
	return paramCont->SetParam(paramName, paramValue);
}

G_EXPORT bool SetParam_LongLongInt(Garosu::ParameterContainer* paramCont, char* paramName, long long int paramValue)
{
	if (paramCont == nullptr) return false;
	return paramCont->SetParam(paramName, paramValue);
}

G_EXPORT bool SetParam_Double(Garosu::ParameterContainer* paramCont, char* paramName, double paramValue)
{
	if (paramCont == nullptr) return false;
	return paramCont->SetParam(paramName, paramValue);
}

G_EXPORT bool SetParam_String(Garosu::ParameterContainer* paramCont, char* paramName, char* paramValue)
{
	if (paramCont == nullptr) return false;
	return paramCont->SetParam(paramName, std::string(paramValue));
}

G_EXPORT bool GetParam_Bool(Garosu::ParameterContainer* paramCont, char* paramName, bool* paramValue)
{
	if (paramCont == nullptr) return false;
	return paramCont->GetParam(paramName, *paramValue);
}

G_EXPORT bool GetParam_VoidPtr(Garosu::ParameterContainer* paramCont, char* paramName, void** paramValue)
{
	if (paramCont == nullptr) return false;
	return paramCont->GetParam(paramName, *paramValue);
}

G_EXPORT bool GetParam_LongLongInt(Garosu::ParameterContainer* paramCont, char* paramName, long long int* paramValue)
{
	if (paramCont == nullptr) return false;
	return paramCont->GetParam(paramName, *paramValue);
}

G_EXPORT bool GetParam_Double(Garosu::ParameterContainer* paramCont, char* paramName, double* paramValue)
{
	if (paramCont == nullptr) return false;
	return paramCont->GetParam(paramName, *paramValue);
}

G_EXPORT bool GetParam_StringSize(Garosu::ParameterContainer* paramCont, char* paramName, unsigned long long int* strSize)
{
	if (paramCont == nullptr) return false;
	std::string str;
	if (paramCont->GetParam(paramName, str) == false) return false;
	*strSize = str.size();
	return true;
}

G_EXPORT bool GetParam_String(Garosu::ParameterContainer* paramCont, char* paramName, char* strBuf, unsigned long long int strSize)
{
	if (paramCont == nullptr) return false;
	std::string str;
	if (paramCont->GetParam(paramName, str) == false) return false;
	strSize = strSize < str.size() ? strSize : str.size();
	strncpy(strBuf, str.c_str(), strSize);
	return true;
}