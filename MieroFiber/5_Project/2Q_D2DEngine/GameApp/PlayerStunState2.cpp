#include "framework.h"
#include "PlayerStunState2.h"

#include "../Engine/EventManager.h"

PlayerStunState2::PlayerStunState2()
{
	AddTransition(&m_StunToIdle);

	EventManager::GetInstance()->RegisterListener(eEventType::P2TransperHP, dynamic_cast<EventListener*>(&m_StunToIdle));
}

void PlayerStunState2::OnEnter()
{
	framework::EVENT_ANIMATION_INFO animationInfo = { L"STUN", false };
	EventManager::GetInstance()->SendEvent(eEventType::P2ChangeAnimation, animationInfo);
}

void PlayerStunState2::OnStay()
{
	framework::EVENT_ANIMATION_INFO animationInfo = { L"STUN", false };
	EventManager::GetInstance()->SendEvent(eEventType::P2KeepAnimation, animationInfo);
}

void PlayerStunState2::OnExit()
{

}
