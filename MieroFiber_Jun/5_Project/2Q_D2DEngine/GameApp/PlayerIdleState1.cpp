#include "framework.h"
#include "PlayerIdleState1.h"

#include "../Engine/EventManager.h"


PlayerIdleState1::PlayerIdleState1()
{
	AddTransition(&m_IdleToStun);
	AddTransition(&m_IdleToMove);

	EventManager::GetInstance()->RegisterListener(eEventType::P1TransperHP, dynamic_cast<EventListener*>(&m_IdleToStun));
	EventManager::GetInstance()->RegisterListener(eEventType::P1TransperMovement, dynamic_cast<EventListener*>(&m_IdleToMove));
}

void PlayerIdleState1::SetDefaultAnimationName()
{
	framework::EVENT_ANIMATION_INFO animationInfo = { L"IDLE_FRONT", false };
	EventManager::GetInstance()->SendEvent(eEventType::P1SetDefaultAnimation, animationInfo);
}

void PlayerIdleState1::OnEnter()
{
	framework::EVENT_ANIMATION_INFO animationInfo;
	if(m_Direction.y == 1.f)
	{
		animationInfo = { L"IDLE_FRONT", false };
	}
	else if(m_Direction.y == -1.f)
	{
		animationInfo = { L"IDLE_BACK", false };
	}
	else if(m_Direction.x == 1.f)
	{
		animationInfo = { L"IDLE_SIDE", false };
	}
	else if (m_Direction.x == -1.f)
	{
		animationInfo = { L"IDLE_SIDE", true };
	}

	EventManager::GetInstance()->SendEvent(eEventType::P1ChangeAnimation, animationInfo);
}

void PlayerIdleState1::OnStay()
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

	EventManager::GetInstance()->SendEvent(eEventType::P1KeepAnimation, animationInfo);
}

void PlayerIdleState1::OnExit()
{
	
}
