#pragma once
#ifndef __GAROSU_PHYSICS_H__
#define __GAROSU_PHYSICS_H__

namespace Garosu {

	class PhysicsFactory
	{
	public:
		static IPhysics* MakeDefaultPhysics(IScheduler* scheduler);
	};

}

#endif
