#pragma once
#ifndef __GAROSU_PHYSICS_H__
#define __GAROSU_PHYSICS_H__

namespace Garosu {
	
	class Physics
	{
	public:
		Physics(void);
		Physics(const Physics&) = delete;
		Physics operator=(const Physics&) = delete;

		virtual ~Physics(void);
	};

}

#endif
