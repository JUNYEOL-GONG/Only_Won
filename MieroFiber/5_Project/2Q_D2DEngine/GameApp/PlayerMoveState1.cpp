#include "framework.h"
#include "PlayerMoveState1.h"

#include "../Engine/EventManager.h"

PlayerMoveState1::PlayerMoveState1()
{
	AddTransition(&m_MoveToStun);
	AddTransition(&m_MoveToIdle);

	EventManager::GetInstance()->RegisterListener(eEventType::P1TransperHP, dynamic_cast<EventListener*>(&m_MoveToStun));
	EventManager::GetInstance()->RegisterListener(eEventType::P1TransperMovement, dynamic_cast<EventListener*>(&m_MoveToIdle));
}

void PlayerMoveState1::OnEnter()
{
	framework::EVENT_ANIMATION_INFO animationInfo;

	if (m_Direction.y == 1.f)
	{
		animationInfo = { L"MOVE_FRONT", false };
	}
	else if (m_Direction.y == -1.f)
	{
		animationInfo = { L"MOVE_BACK", false };
	}
	else if (m_Direction.x == 1.f)
	{
		animationInfo = { L"MOVE_SIDE", false };
	}
	else if (m_Direction.x == -1.f)
	{
		animationInfo = { L"MOVE_SIDE", true };
	}

	EventManager::GetInstance()->SendEvent(eEventType::P1ChangeAnimation, animationInfo);
}

void PlayerMoveState1::OnStay()
{
	framework::EVENT_ANIMATION_INFO animationInfo;
	if (m_Direction.y == 1.f)
	{
		animationInfo = { L"MOVE_FRONT", false };
	}
	else if (m_Direction.y == -1.f)
	{
		animationInfo = { L"MOVE_BACK", false };
	}
	else if (m_Direction.x == 1.f)
	{
		animationInfo = { L"MOVE_SIDE", false };
	}
	else if (m_Direction.x == -1.f)
	{
		animationInfo = { L"MOVE_SIDE", true };
	}

	EventManager::GetInstance()->SendEvent(eEventType::P1KeepAnimation, animationInfo);
}

void PlayerMoveState1::OnExit()
{
	
}
