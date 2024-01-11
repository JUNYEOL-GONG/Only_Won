#include "framework.h"
#include "PlayerTransition3.h"

#include "../Engine/Event.h"
#include "../Engine/EventManager.h"

void PlayerTransition3::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::P3TransperMovement)
	{
		m_MoveDirection = event->movementInfo.moveDirection;
		m_LookDirection = event->movementInfo.lookDirection;
		return;
	}
	if (event->eventID == eEventType::P3TransperHP)
	{
		m_HP = event->hp;
	}
}