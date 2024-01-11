#include "framework.h"
#include "PlayerTransition1.h"

#include "../Engine/Event.h"
#include "../Engine/EventManager.h"

void PlayerTransition1::HandleEvent(Event* event)
{
	if(event->eventID == eEventType::P1TransperMovement)
	{
		m_MoveDirection = event->movementInfo.moveDirection;
		m_LookDirection = event->movementInfo.lookDirection;
		return;
	}
	if(event->eventID == eEventType::P1TransperHP)
	{
		m_HP = event->hp;
	}
}