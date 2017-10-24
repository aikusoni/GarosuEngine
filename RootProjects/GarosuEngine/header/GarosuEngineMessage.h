#pragma once
#ifndef __GAROSU_ENGINE_MESSAGE_H__
#define __GAROSU_ENGINE_MESSAGE_H__

namespace Garosu
{
	enum class EngineMessageId : unsigned int
	{
		Default,
		SetApplicationStoragePath, // use StringMessage
		SetVideoOutputHandle,
	};

	class BaseMessage
	{
	public:
		BaseMessage(EngineMessageId msgId)
			: mEventId(msgId) {}

		EngineMessageId mEventId;
	};

	class StringMessage : public BaseMessage
	{
	public:
		StringMessage(EngineMessageId msgId, const std::string&& str)
			: BaseMessage(msgId), mStr(str) {}

		const std::string mStr;
	};
}

extern "C"
{
	__declspec(dllexport) Garosu::BaseMessage* MakeBaseMessage(Garosu::EngineMessageId);

	__declspec(dllexport) Garosu::StringMessage* MakeStringMessage(Garosu::EngineMessageId, char* charArray);

	__declspec(dllexport) bool DeleteMessage(Garosu::BaseMessage* message);
}

#endif