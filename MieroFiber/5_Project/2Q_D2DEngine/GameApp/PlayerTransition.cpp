#include "framework.h"
#include "PlayerTransition.h"

#include "../Engine/Event.h"
#include "../Engine/EventManager.h"

void PlayerTransition::HandleEvent(Event* event)
{
	if(event->eventID == eEventType::TransperMovement)
	{
		m_MoveDirection = event->movementInfo.moveDirection;
		m_LookDirection = event->movementInfo.lookDirection;
	}
}
