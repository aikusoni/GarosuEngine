#include <GarosuTypedef.h>
#include "GarosuFSM.h"

namespace Garosu
{
	namespace FSM
	{

		StateMachine::StateMachine(void)
			: mCurState(0)
		{

		}

		StateMachine::~StateMachine(void)
		{

		}

		bool StateMachine::SetStartStateCode(const StateCode&)
		{

			return true;
		}

		bool StateMachine::ApplyState(const StateCode&, BaseState*)
		{

			return true;
		}

		bool StateMachine::RemoveState(const StateCode&)
		{

			return true;
		}

		StateCode StateMachine::Request(void)
		{
			try
			{
				mCurState = mStateMap[mCurState]->Handle();
			}
			catch (std::out_of_range)
			{

			}
			return mCurState;
		}

	}

}