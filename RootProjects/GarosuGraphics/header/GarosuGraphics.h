#pragma once
#ifndef __GAROSU_GRAPHICS_H__
#define __GAROSU_GRAPHICS_H__

namespace Garosu
{

	class GraphicsFactory
	{
	public :
		static IGraphics* MakeDefaultGraphics(IScheduler*);
	};
	
}

#endif