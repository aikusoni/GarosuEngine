#pragma once
#ifndef __GAROSU_GRAPHICS_H__
#define __GAROSU_GRAPHICS_H__

// This library uses directx12

namespace Garosu
{

	class GraphicsFactory
	{
	public :
		static IGraphics* MakeDefaultGraphics(IScheduler*);
	};
	
}

#endif