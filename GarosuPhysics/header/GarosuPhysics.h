#pragma once
#ifndef __GAROSU_PHYSICS_H__
#define __GAROSU_PHYSICS_H__

namespace Garosu {

	enum class PhysicsError
	{
		OK,
		ERROR,
	};
	
	class Physics
	{
	public:
		Physics(IScheduler* scheduler);
		Physics(const Physics&) = delete;
		Physics operator=(const Physics&) = delete;

		virtual ~Physics(void);

		PhysicsError Initialize(void);
	};

}

#endif
