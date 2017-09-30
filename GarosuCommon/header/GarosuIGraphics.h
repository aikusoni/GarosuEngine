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
	
	class IGraphicsCallback
	{
	public:
		virtual void operator()(GraphicsCallbackMessageId) = 0;
	};

	class IGraphics
	{
	public:
	};

}

#endif