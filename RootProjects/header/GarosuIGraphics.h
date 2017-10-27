#pragma once
#ifndef __GAROSU_I_GRAPHICS_H__
#define __GAROSU_I_GRAPHICS_H__

#include <GarosuParameterContainer.h>

namespace Garosu
{

	enum class GraphicsError
	{
		OK,
		ERROR,
		ERROR_MESSAGE_NOT_HANDLED
	};

	enum class GraphicsMessageId : unsigned int
	{
		None = 0u,
		SetVideoHandle
	};

	class GraphicsMessage : public ParameterContainer
	{
	public:
		GraphicsMessage(GraphicsMessageId msgId) : mEvtId(msgId) {}

		GraphicsMessageId mEvtId;
	};

	enum class GraphicsEventId : unsigned int
	{

	};

	class GraphicsEvent : public ParameterContainer
	{
	public:
		GraphicsEvent(GraphicsEventId evtId) : mEvtId(evtId) {}

		GraphicsEventId mEvtId;
	};
	
	using GraphicsCallback = bool(GraphicsEvent*);

	class IGraphics
	{
	public:
		virtual GraphicsError Initialize(void) = 0;
		virtual GraphicsError Finalize(void) = 0;

		virtual GraphicsError SendMessage(GraphicsMessage*) = 0;
		virtual GraphicsError RegisterCallback(GraphicsCallback* callback) = 0;
	};

}

#endif