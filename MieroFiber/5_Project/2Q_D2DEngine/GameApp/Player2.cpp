#include "framework.h"
#include "Player2.h"

#include "PlayerFSM2.h"

#include "../Engine/EventManager.h"
#include "../Engine/AnimationComponent.h"
#include "../Engine/FSMComponent.h"

Player2::~Player2()
{
	delete m_pPlayerFSM;
}

bool Player2::Initialize()
{
	bool res = __super::Initialize();
	assert(res);

	SetLocation(ScreenWidth / 2.f + 300.f, ScreenHeight / 2.f);

	m_pPlayerFSM = new PlayerFSM2;
	m_pFSMComponent->AttachFSM(m_pPlayerFSM);

	EventManager::GetInstance()->RegisterListener(eEventType::P2SetDefaultAnimation, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::P2KeepAnimation, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::P2ChangeAnimation, dynamic_cast<EventListener*>(this));

	return true;
}

void Player2::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::P2KeepAnimation)
	{
		m_pAnimComponent->KeepAnimation(event->animationInfo.flip);
		return;
	}
	if (event->eventID == eEventType::P2ChangeAnimation)
	{
		m_pAnimComponent->ChangeAnimation(event->animationInfo.animationName, event->animationInfo.flip);
		return;
	}
	if (event->eventID == eEventType::P2SetDefaultAnimation)
	{
		m_pAnimComponent->SetAnimationName(event->animationInfo.animationName);
		return;
	}
}
