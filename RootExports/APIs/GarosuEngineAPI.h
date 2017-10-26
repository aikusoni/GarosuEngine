#pragma once
#ifndef __GAROSU_ENGINE_H__
#define __GAROSU_ENGINE_H__

#include <memory>

namespace Garosu
{

	class ParameterContainer
	{
	public:
		ParameterContainer(void);
		virtual ~ParameterContainer(void);

		bool SetParam(std::string paramName, bool paramValue);
		bool SetParam(std::string paramName, void* paramValue);
		bool SetParam(std::string paramName, long long paramValue);
		bool SetParam(std::string paramName, double paramValue);
		bool SetParam(std::string paramName, std::string paramValue);

		bool GetParam(std::string paramName, bool& paramValue);
		bool GetParam(std::string paramName, void*& paramValue);
		bool GetParam(std::string paramName, long long& paramValue);
		bool GetParam(std::string paramName, double& paramValue);
		bool GetParam(std::string paramName, std::string& paramValue);

	private:
		class impl;
		std::unique_ptr<impl> pImpl;
	};

	/////
	// Message (you can control engine with messages)
	//
	// if you want to send commands to the engine,
	// call SendMessage function (of the engine).
	/////
	enum class EngineMessageId : unsigned int
	{
		None = 0u,
		SetApplicationStoragePath,
	};

	class BaseMessage
	{
	public:
		BaseMessage(EngineMessageId msgId);

		EngineMessageId mMsgId;
		ParameterContainer mParams;
	};

	/////
	// Event (for Callback)
	//
	// engine will call registered callback function if event occured.
	// event arguments of callback contain eventid(EngineEventId) and more information parameters.
	/////
	enum class EngineEventId : unsigned int
	{
		None = 0u,
	};

	class BaseEvent
	{
	public:
		BaseEvent(EngineEventId eventId);

		EngineEventId mEvtId;
		ParameterContainer params;
	};

	//
	using EngineCallback = bool(BaseEvent*);

	/////
	// Engine Interface
	//
	// EngineFactory returns engine instance that implements this interface.
	// if you wanna use the engine with static library, use this interface,
	// than you wanna use the engine with dynamic library, use exported wrapper functions.
	/////
	class IEngine
	{
	public:
		virtual bool Initialize(void) = 0;
		virtual bool Finalize(void) = 0;

		virtual bool SendMessage(BaseMessage*) = 0;
		virtual bool RegisterCallback(EngineCallback* callback) = 0;
	};

}

extern "C" {
	///// Engine
	__declspec(dllexport) Garosu::IEngine* MakeGarosuEngine(void);
	__declspec(dllexport) bool DeleteGarosuEngine(Garosu::IEngine*);

	// IEngine Functions
	__declspec(dllexport) bool Initialize(Garosu::IEngine*);
	__declspec(dllexport) bool Finalize(Garosu::IEngine*);
	__declspec(dllexport) bool SendMessage(Garosu::IEngine*, Garosu::BaseMessage*);
	__declspec(dllexport) bool RegisterCallback(Garosu::IEngine*, Garosu::EngineCallback*);

	// 
}

#endif