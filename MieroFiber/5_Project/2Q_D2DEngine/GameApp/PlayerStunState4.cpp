#include "framework.h"
#include "PlayerStunState4.h"

#include "../Engine/EventManager.h"

PlayerStunState4::PlayerStunState4()
{
	AddTransition(&m_StunToIdle);

	EventManager::GetInstance()->RegisterListener(eEventType::P4TransperHP, dynamic_cast<EventListener*>(&m_StunToIdle));
}

void PlayerStunState4::OnEnter()
{
	framework::EVENT_ANIMATION_INFO animationInfo = { L"STUN", false };
	EventManager::GetInstance()->SendEvent(eEventType::P4ChangeAnimation, animationInfo);
}

void PlayerStunState4::OnStay()
{
	framework::EVENT_ANIMATION_INFO animationInfo = { L"STUN", false };
	EventManager::GetInstance()->SendEvent(eEventType::P4KeepAnimation, animationInfo);
}

void PlayerStunState4::OnExit()
{

}
