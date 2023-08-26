#include "framework.h"
#include "PlayerTransition2.h"

#include "../Engine/Event.h"
#include "../Engine/EventManager.h"

void PlayerTransition2::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::P2TransperMovement)
	{
		m_MoveDirection = event->movementInfo.moveDirection;
		m_LookDirection = event->movementInfo.lookDirection;
		return;
	}
	if (event->eventID == eEventType::P2TransperHP)
	{
		m_HP = event->hp;
	}
}