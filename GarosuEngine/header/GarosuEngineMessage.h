#pragma once
#ifndef __GAROSU_ENGINE_MESSAGE_H__
#define __GAROSU_ENGINE_MESSAGE_H__

namespace Garosu
{
	enum class EngineMessageId
	{
		None
	};

	class BaseEngineMessage
	{
	public:
		BaseEngineMessage(EngineMessageId messageId)
			: mMessageId(messageId)
		{

		}

		virtual ~BaseEngineMessage(void)
		{

		}

		EngineMessageId mMessageId;
	};
}

#endif