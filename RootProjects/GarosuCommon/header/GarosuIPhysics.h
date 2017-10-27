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

	enum class PhysicsMessageId
	{

	};

	enum class PhysicsCallbackMessageId
	{

	};

	class PhysicsMessage
	{
	public:
		PhysicsMessage(PhysicsMessageId msgId) : mEvtId(msgId) {}

		PhysicsMessageId mEvtId;
	};

	class IPhysicsCallback
	{
	public:
		virtual void operator()(PhysicsCallbackMessageId) = 0;
	};

	class IPhysics
	{
	public:
		virtual PhysicsError Initialize(void) = 0;
		virtual PhysicsError Finalize(void) = 0;

		virtual PhysicsError SendMessage(PhysicsMessage*) = 0;
		virtual PhysicsError RegisterCallback(IPhysicsCallback* callback) = 0;
	};

}

#endif
