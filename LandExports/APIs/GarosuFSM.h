#pragma once
#ifndef __GAROSU_FSM_H__
#define __GAROSU_FSM_H__

#include "GarosuTypedef.h"

namespace Garosu
{
	namespace FSM
	{
		using StateCode = u32;

		class BaseState
		{
		public:
			virtual StateCode Handle(void) = 0;
		};

		class StateMachine
		{
		public:
			StateMachine(void);
			StateMachine(const StateMachine&) = delete;
			StateMachine& operator=(const StateMachine&) = delete;

			virtual ~StateMachine(void);

			StateCode GetStateCode(void) { return mCurState; }

			bool SetStartStateCode(const StateCode&);

			bool ApplyState(const StateCode&, BaseState*);
			bool RemoveState(const StateCode&);

			StateCode Request(void);

		private:
			std::map<StateCode, BaseState*> mStateMap;
			StateCode mCurState;
		};
	}
}

#endif
