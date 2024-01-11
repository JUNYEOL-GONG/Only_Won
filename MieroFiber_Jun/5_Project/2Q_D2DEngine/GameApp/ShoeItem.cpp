#include "framework.h"
#include "ShoeItem.h"

#include "PlayerObject.h"

#include "../Engine/PlayerMovement.h"
#include "../Engine/EventManager.h"
#include "../Engine/World.h"
#include "../Engine/SoundManager.h"

float ShoeItem::m_shoeElapsedTime = 0.f;

bool ShoeItem::Initialize()
{
	m_pPlayer = dynamic_cast<PlayerObject*>(m_pOwnerPlayer);
	PlayerMovement* playerMovement = m_pOwnerPlayer->GetComponent<PlayerMovement>();

	int playerNumber = m_pPlayer->GetPlayerNumber();
	if (playerNumber == 0)
		SoundManager::GetInstance()->PlayMusic(eSoundList::SpeedEffect, eSoundChannel::Player1State, 1.f);
	else if (playerNumber == 1)																		 
		SoundManager::GetInstance()->PlayMusic(eSoundList::SpeedEffect, eSoundChannel::Player2State, 1.f);
	else if (playerNumber == 2)																		 
		SoundManager::GetInstance()->PlayMusic(eSoundList::SpeedEffect, eSoundChannel::Player3State, 1.f);
	else if (playerNumber == 3)																		 
		SoundManager::GetInstance()->PlayMusic(eSoundList::SpeedEffect, eSoundChannel::Player4State, 1.f);

	// 이동속도 두배로 만들기
	playerMovement->SetOriginalVelocityLimit(2.f);
	return true;
}

void ShoeItem::Update(const float deltaTime)
{
	PlayerMovement* playerMovement = m_pOwnerPlayer->GetComponent<PlayerMovement>();

	m_shoeElapsedTime += deltaTime;

	if(m_shoeElapsedTime > 5.f)
	{
		// 이동속도 원래대로
		playerMovement->SetOriginalVelocityLimit(0.5f);
		m_shoeElapsedTime = 0;
		EventManager::GetInstance()->SendEvent(eEventType::DeleteGameObject, GROUP_TYPE::ITEM, this);

		m_pPlayer->SetReinforcedNothing();
	}
	else if(m_pPlayer->IsStun() || m_pPlayer->IsFalling())
	{
		// 지속시간 끝나기 전에 스턴 상태가 되면
		// 이동속도 원래대로
		playerMovement->SetOriginalVelocityLimit(0.5f);
		EventManager::GetInstance()->SendEvent(eEventType::DeleteGameObject, GROUP_TYPE::ITEM, this);

		m_pPlayer->SetReinforcedNothing();
	}
}