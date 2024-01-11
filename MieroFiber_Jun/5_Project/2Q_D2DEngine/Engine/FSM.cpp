#include "pch.h"
#include "FSM.h"

#include "FSMState.h"

void FSM::AddState(const std::wstring name, FSMState* state)
{
	m_States.insert(std::pair<std::wstring, FSMState*>(name, state));
	state->SetName(name);
	state->SetParentFSM(this);
}

void FSM::SetDefaultState(FSMState* defaultState)
{
	m_pCurState = defaultState;
}

void FSM::Update(const float deltaTime)
{
	// 현재 상태에 달린 Transition들을 체크
	if (m_pCurState->CheckTransition())
	{
		// 만족하는 Transition이 있으면 상태 전환
		m_pCurState = m_pNextState;
		m_pCurState->OnEnter();
	}
	else
	{
		// 만족하는 Transition이 없으면 상태 지속
		m_pCurState->OnStay();
	}
}
