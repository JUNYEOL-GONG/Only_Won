#include "pch.h"
#include "FSMState.h"

#include "AnimationComponent.h"
#include "FSM.h"
#include "FSMTransition.h"

void FSMState::SetParentFSM(FSM* parentFSM)
{
	m_pParentFSM = parentFSM;
}

bool FSMState::CheckTransition()
{
	for (auto transition : m_Transitions)
	{
		std::wstring nextStateName = (*transition)().first;
		m_Direction = (*transition)().second;

		if (!nextStateName.empty())
		{
			// state ��Ͽ��� find
			FSMState* nextState = m_pParentFSM->FindState(nextStateName);
			m_pParentFSM->SetNextState(nextState);
			return true;
		}
	}

	return false;
}