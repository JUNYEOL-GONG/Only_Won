#include "framework.h"
#include "PlayerStunState1.h"

#include "../Engine/EventManager.h"

PlayerStunState1::PlayerStunState1()
{
	AddTransition(&m_StunToIdle);

	EventManager::GetInstance()->RegisterListener(eEventType::P1TransperHP, dynamic_cast<EventListener*>(&m_StunToIdle));
}

void PlayerStunState1::OnEnter()
{
	framework::EVENT_ANIMATION_INFO animationInfo = { L"STUN", false };
	EventManager::GetInstance()->SendEvent(eEventType::P1ChangeAnimation, animationInfo);
}

void PlayerStunState1::OnStay()
{
	framework::EVENT_ANIMATION_INFO animationInfo = { L"STUN", false };
	EventManager::GetInstance()->SendEvent(eEventType::P1KeepAnimation, animationInfo);
}

void PlayerStunState1::OnExit()
{

}
