#pragma once
#ifndef __GAROSU_I_PHYSICS_H__
#define __GAROSU_I_PHYSICS_H__

namespace Garosu
{

	enum class PhysicsError
	{
		OK,
		ERROR,
	};

	class IPhysics
	{
	public:
		virtual PhysicsError Initialize(void) = 0;
		virtual PhysicsError Finalize(void) = 0;
	};

}

#endif
