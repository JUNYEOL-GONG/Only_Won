#include "framework.h"
#include "PlayerIdleState2.h"

#include "../Engine/EventManager.h"

PlayerIdleState2::PlayerIdleState2()
{
	AddTransition(&m_IdleToStun);
	AddTransition(&m_IdleToMove);

	EventManager::GetInstance()->RegisterListener(eEventType::P2TransperHP, dynamic_cast<EventListener*>(&m_IdleToStun));
	EventManager::GetInstance()->RegisterListener(eEventType::P2TransperMovement, dynamic_cast<EventListener*>(&m_IdleToMove));
}

void PlayerIdleState2::SetDefaultAnimationName()
{
	framework::EVENT_ANIMATION_INFO animationInfo = { L"IDLE_FRONT", false };
	EventManager::GetInstance()->SendEvent(eEventType::P2SetDefaultAnimation, animationInfo);
}

void PlayerIdleState2::OnEnter()
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

	EventManager::GetInstance()->SendEvent(eEventType::P2ChangeAnimation, animationInfo);
}

void PlayerIdleState2::OnStay()
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

	EventManager::GetInstance()->SendEvent(eEventType::P2KeepAnimation, animationInfo);
}

void PlayerIdleState2::OnExit()
{
}