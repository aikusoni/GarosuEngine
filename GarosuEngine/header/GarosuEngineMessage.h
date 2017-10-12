#pragma once
#ifndef __GAROSU_ENGINE_MESSAGE_H__
#define __GAROSU_ENGINE_MESSAGE_H__

namespace Garosu
{
	enum class EngineMessageId
	{
		Default,
		SetApplicationStoragePath, // use StringMessage
	};

	class BaseMessage
	{
	public:
		BaseMessage(EngineMessageId msgId)
			: mMessageId(msgId) {}

		EngineMessageId mMessageId;
	};

	class StringMessage : public BaseMessage
	{
	public:
		StringMessage(EngineMessageId msgId, const String& str)
			: BaseMessage(msgId), mStr(str) {}

		const String& mStr;
	};
}

#endif