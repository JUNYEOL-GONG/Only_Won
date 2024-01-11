#include "pch.h"
#include "FSMComponent.h"

#include "AnimationComponent.h"
#include "FSM.h"
#include "FSMState.h"

void FSMComponent::AttachFSM(FSM* _FSM)
{
	m_FSM = _FSM;
	m_FSM->GetCurState()->SetDefaultAnimationName();
}

void FSMComponent::Update(const float deltaTime)
{
	m_FSM->Update(deltaTime);
}
