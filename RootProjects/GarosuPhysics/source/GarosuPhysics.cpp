#include <GarosuTypedef.h>

#include <GarosuTask.h>
#include <GarosuIScheduler.h>
#include <GarosuIPhysics.h>

#include "GarosuPhysics.h"

#include <GarosuLog.h>

namespace Garosu {

	class Physics : public IPhysics
	{
	public:
		Physics(IScheduler* scheduler);
		Physics(const Physics&) = delete;
		Physics& operator=(const Physics&) = delete;

		virtual ~Physics(void);

		virtual PhysicsError Initialize(void);
		virtual PhysicsError Finalize(void);

		virtual PhysicsError SendMessage(PhysicsMessage*);
		virtual PhysicsError RegisterCallback(IPhysicsCallback*);
	};

	Physics::Physics(IScheduler* scheduler)
	{

	}

	Physics::~Physics(void)
	{

	}

	PhysicsError Physics::Initialize(void)
	{
		LOGQC("We need to make physics engine.");

		return PhysicsError::OK;
	}

	PhysicsError Physics::Finalize(void)
	{

		return PhysicsError::OK;
	}

	PhysicsError Physics::SendMessage(PhysicsMessage* message)
	{

		return PhysicsError::OK;
	}

	PhysicsError Physics::RegisterCallback(IPhysicsCallback* callback)
	{

		return PhysicsError::OK;
	}

	IPhysics* PhysicsFactory::MakeDefaultPhysics(IScheduler* scheduler)
	{
		return new Physics(scheduler);
	}

}