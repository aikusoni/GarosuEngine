#include <memory>
#include <string>

#include <SchedulerInterface.h>

#include "GarosuPhysics.h"

#include <GarosuLog.h>

namespace Garosu {

	Physics::Physics(IScheduler* scheduler)
	{

	}

	Physics::~Physics(void)
	{

	}

	PhysicsError Physics::Initialize(void)
	{
		LOGC("We need to make physics engine.");

		return PhysicsError::OK;
	}

}