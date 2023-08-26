#include "framework.h"
#include "PlayerMoveState4.h"

#include "../Engine/EventManager.h"

PlayerMoveState4::PlayerMoveState4()
{
	AddTransition(&m_MoveToStun);
	AddTransition(&m_MoveToIdle);

	EventManager::GetInstance()->RegisterListener(eEventType::P4TransperHP, dynamic_cast<EventListener*>(&m_MoveToStun));
	EventManager::GetInstance()->RegisterListener(eEventType::P4TransperMovement, dynamic_cast<EventListener*>(&m_MoveToIdle));
}

void PlayerMoveState4::OnEnter()
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

	EventManager::GetInstance()->SendEvent(eEventType::P4ChangeAnimation, animationInfo);
}

void PlayerMoveState4::OnStay()
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

	EventManager::GetInstance()->SendEvent(eEventType::P4KeepAnimation, animationInfo);
}

void PlayerMoveState4::OnExit()
{

}