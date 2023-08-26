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

	// �̵��ӵ� �ι�� �����
	playerMovement->SetOriginalVelocityLimit(2.f);
	return true;
}

void ShoeItem::Update(const float deltaTime)
{
	PlayerMovement* playerMovement = m_pOwnerPlayer->GetComponent<PlayerMovement>();

	m_shoeElapsedTime += deltaTime;

	if(m_shoeElapsedTime > 5.f)
	{
		// �̵��ӵ� �������
		playerMovement->SetOriginalVelocityLimit(0.5f);
		m_shoeElapsedTime = 0;
		EventManager::GetInstance()->SendEvent(eEventType::DeleteGameObject, GROUP_TYPE::ITEM, this);

		m_pPlayer->SetReinforcedNothing();
	}
	else if(m_pPlayer->IsStun() || m_pPlayer->IsFalling())
	{
		// ���ӽð� ������ ���� ���� ���°� �Ǹ�
		// �̵��ӵ� �������
		playerMovement->SetOriginalVelocityLimit(0.5f);
		EventManager::GetInstance()->SendEvent(eEventType::DeleteGameObject, GROUP_TYPE::ITEM, this);

		m_pPlayer->SetReinforcedNothing();
	}
}