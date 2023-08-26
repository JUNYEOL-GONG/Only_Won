#include "framework.h"
#include "PlayerTransition4.h"

#include "../Engine/Event.h"
#include "../Engine/EventManager.h"

void PlayerTransition4::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::P4TransperMovement)
	{
		m_MoveDirection = event->movementInfo.moveDirection;
		m_LookDirection = event->movementInfo.lookDirection;
		return;
	}
	if (event->eventID == eEventType::P4TransperHP)
	{
		m_HP = event->hp;
	}
}