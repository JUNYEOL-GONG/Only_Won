#include "framework.h"
#include "TransparencyItem.h"


#include "PlayerObject.h"

#include "../Engine/PlayerMovement.h"
#include "../Engine/EventManager.h"
#include "../Engine/SoundManager.h"

float TransparencyItem::transElapsedTime = 0.f;
float TransparencyItem::transfootprintElapsedTime = 0.f;

bool TransparencyItem::Initialize()
{
	m_pPlayer = dynamic_cast<PlayerObject*>(m_pOwnerPlayer);
	int playerNumber = dynamic_cast<PlayerObject*>(m_pOwnerPlayer)->GetPlayerNumber();

	if (playerNumber == 0)
		SoundManager::GetInstance()->PlayMusic(eSoundList::HideEffect, eSoundChannel::Player1State, 1.f);
	else if (playerNumber == 1)																		
		SoundManager::GetInstance()->PlayMusic(eSoundList::HideEffect, eSoundChannel::Player2State, 1.f);
	else if (playerNumber == 2)																		
		SoundManager::GetInstance()->PlayMusic(eSoundList::HideEffect, eSoundChannel::Player3State, 1.f);
	else if (playerNumber == 3)																		
		SoundManager::GetInstance()->PlayMusic(eSoundList::HideEffect, eSoundChannel::Player4State, 1.f);

	m_playerNum = m_pPlayer->GetPlayerNumber();

	m_curPlayerLocation = m_pOwnerPlayer->GetGameObjectLocation();
	m_pPlayerMovement = m_pOwnerPlayer->GetComponent<PlayerMovement>();

	if(m_playerNum == 0)
		EventManager::GetInstance()->SendEvent(eEventType::Player1TransparentStart);
	else if(m_playerNum == 1)
		EventManager::GetInstance()->SendEvent(eEventType::Player2TransparentStart);
	else if (m_playerNum == 2)
		EventManager::GetInstance()->SendEvent(eEventType::Player3TransparentStart);
	else if (m_playerNum == 3)
		EventManager::GetInstance()->SendEvent(eEventType::Player4TransparentStart);

	m_pPlayer->SetTransparencing(true);

    return true;
}

void TransparencyItem::Update(const float deltaTime)
{
	framework::Vector2D lookDirection = m_pPlayerMovement->GetLookDir();


	transElapsedTime += deltaTime;
	transfootprintElapsedTime += deltaTime;

	if(transfootprintElapsedTime > 0.1f && transfootprintElapsedTime < 0.3f)
	{
		if(m_curPlayerLocation.x == 0.f && m_curPlayerLocation.y == 0.f)
		{
			m_curPlayerLocation = m_pOwnerPlayer->GetGameObjectLocation();
			EventManager::GetInstance()->SendEvent(eEventType::FootprintVisible, m_playerNum, m_curPlayerLocation, lookDirection);
		}
	}
	else if (transfootprintElapsedTime > 0.3f)
	{
		EventManager::GetInstance()->SendEvent(eEventType::FootprintInVisible, m_playerNum, m_curPlayerLocation, lookDirection);
		transfootprintElapsedTime = 0;
		m_curPlayerLocation = { 0.f, 0.f };
	}

	if (transElapsedTime > 8.f)
	{
		// 플레이어마다 따로 투명화 끝내줘야함..
		if (m_playerNum == 0)
			EventManager::GetInstance()->SendEvent(eEventType::Player1TransparentEnd);
		else if (m_playerNum == 1)
			EventManager::GetInstance()->SendEvent(eEventType::Player2TransparentEnd);
		else if (m_playerNum == 2)
			EventManager::GetInstance()->SendEvent(eEventType::Player3TransparentEnd);
		else if (m_playerNum == 3)
			EventManager::GetInstance()->SendEvent(eEventType::Player4TransparentEnd);

		EventManager::GetInstance()->SendEvent(eEventType::FootprintInVisible, m_playerNum, m_curPlayerLocation, lookDirection);
		EventManager::GetInstance()->SendEvent(eEventType::DeleteGameObject, GROUP_TYPE::ITEM, this);
		transElapsedTime = 0;
		transfootprintElapsedTime = 0;

		m_pPlayer->SetReinforcedNothing();
		m_pPlayer->SetTransparencing(false);
	}
}