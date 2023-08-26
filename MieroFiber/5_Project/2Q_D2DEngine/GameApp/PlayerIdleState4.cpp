#include "framework.h"
#include "PlayerIdleState4.h"

#include "../Engine/EventManager.h"


PlayerIdleState4::PlayerIdleState4()
{
	AddTransition(&m_IdleToStun);
	AddTransition(&m_IdleToMove);

	EventManager::GetInstance()->RegisterListener(eEventType::P4TransperHP, dynamic_cast<EventListener*>(&m_IdleToStun));
	EventManager::GetInstance()->RegisterListener(eEventType::P4TransperMovement, dynamic_cast<EventListener*>(&m_IdleToMove));
}

void PlayerIdleState4::SetDefaultAnimationName()
{
	framework::EVENT_ANIMATION_INFO animationInfo = { L"IDLE_FRONT", false };
	EventManager::GetInstance()->SendEvent(eEventType::P4SetDefaultAnimation, animationInfo);
}

void PlayerIdleState4::OnEnter()
{
	framework::EVENT_ANIMATION_INFO animationInfo;
	if (m_Direction.y == 1.f)
	{
		animationInfo = { L"IDLE_FRONT", false };
	}
	else if (m_Direction.y == -1.f)
	{
		animationInfo = { L"IDLE_BACK", false };
	}
	else if (m_Direction.x == 1.f)
	{
		animationInfo = { L"IDLE_SIDE", false };
	}
	else if (m_Direction.x == -1.f)
	{
		animationInfo = { L"IDLE_SIDE", true };
	}

	EventManager::GetInstance()->SendEvent(eEventType::P4ChangeAnimation, animationInfo);
}

void PlayerIdleState4::OnStay()
{
	framework::EVENT_ANIMATION_INFO animationInfo;
	if (m_Direction.y == 1.f)
	{
		animationInfo = { L"IDLE_FRONT", false };
	}
	else if (m_Direction.y == -1.f)
	{
		animationInfo = { L"IDLE_BACK", false };
	}
	else if (m_Direction.x == 1.f)
	{
		animationInfo = { L"IDLE_SIDE", false };
	}
	else if (m_Direction.x == -1.f)
	{
		animationInfo = { L"IDLE_SIDE", true };
	}

	EventManager::GetInstance()->SendEvent(eEventType::P4KeepAnimation, animationInfo);
}

void PlayerIdleState4::OnExit()
{

}
