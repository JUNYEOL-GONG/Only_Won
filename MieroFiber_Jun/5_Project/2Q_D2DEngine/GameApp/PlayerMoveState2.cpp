#include "framework.h"
#include "PlayerMoveState2.h"

#include "../Engine/EventManager.h"


PlayerMoveState2::PlayerMoveState2()
{
	AddTransition(&m_MoveToStun);
	AddTransition(&m_MoveToIdle);

	EventManager::GetInstance()->RegisterListener(eEventType::P2TransperHP, dynamic_cast<EventListener*>(&m_MoveToStun));
	EventManager::GetInstance()->RegisterListener(eEventType::P2TransperMovement, dynamic_cast<EventListener*>(&m_MoveToIdle));
}

void PlayerMoveState2::OnEnter()
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

	EventManager::GetInstance()->SendEvent(eEventType::P2ChangeAnimation, animationInfo);
}

void PlayerMoveState2::OnStay()
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

	EventManager::GetInstance()->SendEvent(eEventType::P2KeepAnimation, animationInfo);
}

void PlayerMoveState2::OnExit()
{
}