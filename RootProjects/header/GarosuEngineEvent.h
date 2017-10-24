#pragma once
#ifndef __GAROSU_ENGINE_EVENT_H__
#define __GAROSU_ENGINE_EVENT_H__

namespace Garosu
{

	enum class EngineEventId : unsigned int
	{
		Default,
	};

	class BaseEvent
	{
	public:
		BaseEvent(EngineEventId eventId)
			: mEventId(eventId) {}

		EngineEventId mEventId;
	};

	class StringEvent : public BaseEvent
	{
	public:
		StringEvent(EngineEventId eventId, const std::string&& str)
			: BaseEvent(eventId), mStr(str) {}

		const std::string mStr;
	};

}

extern "C"
{

	__declspec(dllexport) bool GetEventId(Garosu::BaseEvent*, Garosu::EngineEventId* eventId);
	__declspec(dllexport) bool GetStringSize(Garosu::StringEvent*, size_t* pNeedSize);
	__declspec(dllexport) bool GetString(Garosu::StringEvent*, char* buffer, size_t size);

}

#endif