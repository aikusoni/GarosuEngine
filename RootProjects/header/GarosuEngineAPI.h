#pragma once
#ifndef __GAROSU_ENGINE_H__
#define __GAROSU_ENGINE_H__

#include <GarosuParameterContainer.h>

namespace Garosu
{

	/////
	// Message (you can control engine with messages)
	//
	// if you want to send commands to the engine,
	// call SendMessage function (of the engine).
	/////
	enum class EngineMessageId : unsigned int
	{
		None = 0u,
		TestMessage,
		SetApplicationStoragePath,
		SetVideoOutputHandle, // "output_target" : (void*)
	};

	class BaseMessage : public ParameterContainer
	{
	public:
		BaseMessage(EngineMessageId msgId);

		EngineMessageId mEvtId;
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
		InitializeComplete,
	};

	class BaseEvent : public ParameterContainer
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

#define G_EXPORT __declspec(dllexport)

extern "C" {
	// Engine
	G_EXPORT Garosu::IEngine* CreateEngine(void);
	G_EXPORT bool DeleteEngine(Garosu::IEngine*);

	// IEngine Functions
	G_EXPORT bool Initialize(Garosu::IEngine*);
	G_EXPORT bool Finalize(Garosu::IEngine*);
	G_EXPORT bool SendMessage(Garosu::IEngine*, Garosu::BaseMessage*);
	G_EXPORT bool RegisterCallback(Garosu::IEngine*, Garosu::EngineCallback*);

	// Message
	G_EXPORT Garosu::BaseMessage* CreateMessage(unsigned int msgId);
	G_EXPORT bool DeleteMessage(Garosu::BaseMessage*);
	G_EXPORT Garosu::EngineMessageId GetMessageId(Garosu::BaseMessage*);

	// Event
	G_EXPORT Garosu::EngineEventId GetEventId(Garosu::BaseEvent*);

	// Params (message or event)
	G_EXPORT bool SetParam_Bool(Garosu::ParameterContainer*, char* paramName, bool paramValue);
	G_EXPORT bool SetParam_VoidPtr(Garosu::ParameterContainer*, char* paramName, void* paramValue);
	G_EXPORT bool SetParam_LongLongInt(Garosu::ParameterContainer*, char* paramName, long long int paramValue);
	G_EXPORT bool SetParam_Double(Garosu::ParameterContainer*, char* paramName, double paramValue);
	G_EXPORT bool SetParam_String(Garosu::ParameterContainer*, char* paramName, char* paramValue);

	G_EXPORT bool GetParam_Bool(Garosu::ParameterContainer*, char* paramName, bool* paramValue);
	G_EXPORT bool GetParam_VoidPtr(Garosu::ParameterContainer*, char* paramName, void** paramValue);
	G_EXPORT bool GetParam_LongLongInt(Garosu::ParameterContainer*, char* paramName, long long int* paramValue);
	G_EXPORT bool GetParam_Double(Garosu::ParameterContainer*, char* paramName, double* paramValue);
	G_EXPORT bool GetParam_String(Garosu::ParameterContainer* paramCont, char* paramName, char* paramVal);

	G_EXPORT bool GetParam_SizeOfString(Garosu::ParameterContainer*, char* paramName, unsigned long long int* strSize);
	G_EXPORT bool GetParam_StringWithSize(Garosu::ParameterContainer*, char* paramName, char* strBuf, unsigned long long int strSize);
}

#endif