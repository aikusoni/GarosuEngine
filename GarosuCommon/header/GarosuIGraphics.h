#pragma once
#ifndef __GAROSU_I_GRAPHICS_H__
#define __GAROSU_I_GRAPHICS_H__

namespace Garosu
{

	enum class GraphicsError
	{
		OK,
		ERROR,
	};

	enum class GraphicsMessageId
	{

	};

	enum class GraphicsCallbackMessageId
	{

	};

	class GraphicsMessage
	{
	public:
		GraphicsMessage(GraphicsMessageId msgId) : mMsgId(msgId) {}

		GraphicsMessageId mMsgId;
	};
	
	class IGraphicsCallback
	{
	public:
		virtual void operator()(GraphicsCallbackMessageId) = 0;
	};

	class IGraphics
	{
	public:
		virtual GraphicsError Initialize(void) = 0;
		virtual GraphicsError Finalize(void) = 0;

		virtual GraphicsError SendMessage(GraphicsMessage*) = 0;
		virtual GraphicsError RegisterCallback(IGraphicsCallback* callback) = 0;
	};

}

#endif