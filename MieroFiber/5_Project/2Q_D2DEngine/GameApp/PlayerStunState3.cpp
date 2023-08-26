#include "framework.h"
#include "PlayerStunState3.h"

#include "../Engine/EventManager.h"

PlayerStunState3::PlayerStunState3()
{
	AddTransition(&m_StunToIdle);

	EventManager::GetInstance()->RegisterListener(eEventType::P3TransperHP, dynamic_cast<EventListener*>(&m_StunToIdle));
}

void PlayerStunState3::OnEnter()
{
	framework::EVENT_ANIMATION_INFO animationInfo = { L"STUN", false };
	EventManager::GetInstance()->SendEvent(eEventType::P3ChangeAnimation, animationInfo);
}

void PlayerStunState3::OnStay()
{
	framework::EVENT_ANIMATION_INFO animationInfo = { L"STUN", false };
	EventManager::GetInstance()->SendEvent(eEventType::P3KeepAnimation, animationInfo);
}

void PlayerStunState3::OnExit()
{

}
