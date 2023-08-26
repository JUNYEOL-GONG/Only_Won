#include "framework.h"
#include "PlayerMoveState3.h"

#include "../Engine/EventManager.h"

PlayerMoveState3::PlayerMoveState3()
{
	AddTransition(&m_MoveToStun);
	AddTransition(&m_MoveToIdle);

	EventManager::GetInstance()->RegisterListener(eEventType::P3TransperHP, dynamic_cast<EventListener*>(&m_MoveToStun));
	EventManager::GetInstance()->RegisterListener(eEventType::P3TransperMovement, dynamic_cast<EventListener*>(&m_MoveToIdle));
}

void PlayerMoveState3::OnEnter()
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

	EventManager::GetInstance()->SendEvent(eEventType::P3ChangeAnimation, animationInfo);
}

void PlayerMoveState3::OnStay()
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

	EventManager::GetInstance()->SendEvent(eEventType::P3KeepAnimation, animationInfo);
}

void PlayerMoveState3::OnExit()
{

}