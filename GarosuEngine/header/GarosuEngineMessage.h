#pragma once
#ifndef __GAROSU_ENGINE_MESSAGE_H__
#define __GAROSU_ENGINE_MESSAGE_H__

namespace Garosu
{
	enum class EngineMessageId
	{
		Default
	};

	class BaseEngineMessage
	{
	public:
		BaseEngineMessage()
			: mMessageId(EngineMessageId::Default)
		{

		}

		virtual ~BaseEngineMessage(void)
		{

		}

		EngineMessageId mMessageId;
	};
}

#endif