#pragma once
#ifndef __GAROSU_GRAPHICS_H__
#define __GAROSU_GRAPHICS_H__

// This library uses directx12

namespace Garosu {

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

	class IGraphicsCallback
	{
	public:
		virtual void operator()(GraphicsCallbackMessageId) = 0;
	};

	class Graphics
	{
	public:
		Graphics(IScheduler*);
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;

		~Graphics(void);

		GraphicsError Initialize(void);
		GraphicsError Start(void);
		GraphicsError Stop(void);

		GraphicsError SendMessage(GraphicsMessageId messageId);
		GraphicsError RegisterCallback(IGraphicsCallback* callback);

	private:
		class impl;
		uptr<impl> pImpl;
	};

}

#endif