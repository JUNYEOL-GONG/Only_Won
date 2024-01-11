#include "framework.h"
#include "PlayerIdleState3.h"

#include "../Engine/EventManager.h"


PlayerIdleState3::PlayerIdleState3()
{
	AddTransition(&m_IdleToStun);
	AddTransition(&m_IdleToMove);

	EventManager::GetInstance()->RegisterListener(eEventType::P3TransperHP, dynamic_cast<EventListener*>(&m_IdleToStun));
	EventManager::GetInstance()->RegisterListener(eEventType::P3TransperMovement, dynamic_cast<EventListener*>(&m_IdleToMove));
}

void PlayerIdleState3::SetDefaultAnimationName()
{
	framework::EVENT_ANIMATION_INFO animationInfo = { L"IDLE_FRONT", false };
	EventManager::GetInstance()->SendEvent(eEventType::P3SetDefaultAnimation, animationInfo);
}

void PlayerIdleState3::OnEnter()
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

	EventManager::GetInstance()->SendEvent(eEventType::P3ChangeAnimation, animationInfo);
}

void PlayerIdleState3::OnStay()
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

	EventManager::GetInstance()->SendEvent(eEventType::P3KeepAnimation, animationInfo);
}

void PlayerIdleState3::OnExit()
{

}
