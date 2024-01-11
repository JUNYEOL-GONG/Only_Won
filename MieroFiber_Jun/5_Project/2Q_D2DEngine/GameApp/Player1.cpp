#include "framework.h"
#include "Player1.h"

#include "PlayerFSM1.h"

#include "../Engine/EventManager.h"
#include "../Engine/AnimationComponent.h"
#include "../Engine/FSMComponent.h"

Player1::~Player1()
{
	delete m_pPlayerFSM;
}

bool Player1::Initialize()
{
	bool res = __super::Initialize();
	assert(res);

	SetLocation(ScreenWidth / 2.f - 300.f, ScreenHeight / 2.f);

	m_pPlayerFSM = new PlayerFSM1;
	m_pFSMComponent->AttachFSM(m_pPlayerFSM);

	EventManager::GetInstance()->RegisterListener(eEventType::P1SetDefaultAnimation, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::P1KeepAnimation, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::P1ChangeAnimation, dynamic_cast<EventListener*>(this));

	return true;
}

void Player1::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::P1KeepAnimation)
	{
		m_pAnimComponent->KeepAnimation(event->animationInfo.flip);
		return;
	}
	if (event->eventID == eEventType::P1ChangeAnimation)
	{
		m_pAnimComponent->ChangeAnimation(event->animationInfo.animationName, event->animationInfo.flip);
		return;
	}
	if (event->eventID == eEventType::P1SetDefaultAnimation)
	{
		m_pAnimComponent->SetAnimationName(event->animationInfo.animationName);
		return;
	}
}
