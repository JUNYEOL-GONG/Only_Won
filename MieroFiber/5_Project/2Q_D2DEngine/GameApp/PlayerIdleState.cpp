#include "framework.h"
#include "PlayerIdleState.h"

#include "PlayerTransition.h"

#include "../Engine/EventManager.h"

PlayerIdleState::PlayerIdleState()
{
	AddTransition(&m_IdleToMove);

	EventManager::GetInstance()->RegisterListener(eEventType::TransperMovement, dynamic_cast<EventListener*>(&m_IdleToMove));
}

void PlayerIdleState::OnEnter()
{
	framework::EVENT_ANIMATION_INFO animationInfo = { L"IDLE", m_bFlip };
	EventManager::GetInstance()->SendEvent(eEventType::ChangeAnimation, animationInfo);
}

void PlayerIdleState::OnStay()
{
	framework::EVENT_ANIMATION_INFO animationInfo = { L"IDLE", m_bFlip };
	EventManager::GetInstance()->SendEvent(eEventType::KeepAnimation, animationInfo);
}

void PlayerIdleState::OnExit()
{
}
