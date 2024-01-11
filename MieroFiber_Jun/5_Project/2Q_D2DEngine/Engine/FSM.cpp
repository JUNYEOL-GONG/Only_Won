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
	// ���� ���¿� �޸� Transition���� üũ
	if (m_pCurState->CheckTransition())
	{
		// �����ϴ� Transition�� ������ ���� ��ȯ
		m_pCurState = m_pNextState;
		m_pCurState->OnEnter();
	}
	else
	{
		// �����ϴ� Transition�� ������ ���� ����
		m_pCurState->OnStay();
	}
}
