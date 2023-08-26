#include "framework.h"
#include "TrapItem.h"

#include "PlayerObject.h"

#include "../Engine/TextureComponent.h"
#include "../Engine/BoxCollider2D.h"
#include "../Engine/EventManager.h"
#include "../Engine/SoundManager.h"

bool TrapItem::Initialize()
{
	m_Duration = 1.f;

	m_pTextureComponent = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pTextureComponent->SetTextureAsset(L"../Resource/함정.png", L"TrapItem");
	m_pTextureComponent->SetRelativeLocation(m_pOwnerPlayer->GetGameObjectLocation().x, m_pOwnerPlayer->GetGameObjectLocation().y);
	SetRootComponent(m_pTextureComponent);

	m_pCollider = CreateComponent<BoxCollider2D>(L"Collider2D");
	m_pCollider->SetCollisionType(COLLISION_TYPE::TRIGGER);
	m_pCollider->SetExtend(35.f, 20.f);
	m_pCollider->AttachToComponent(m_pTextureComponent);

	bool res = __super::Initialize();
	assert(res);

	return true;
}

void TrapItem::Update(const float deltaTime)
{
	m_DelayTime += deltaTime;
	if(m_DelayTime >= 1.f)
	{
		m_Active = true;
	}

	if(m_bAffected)
	{
		m_ProgressTime += deltaTime;
		if(m_ProgressTime >= 1.5f)
		{
			m_DelayTime = 0.f;
			m_ProgressTime = 0.f;
			m_pTextureComponent->SetVisible(false);
			// static_cast<PlayerObject*>(m_pAttackedPlayer)->TurnToStun();
			// static_cast<PlayerObject*>(m_pAttackedPlayer)->MoveToSpawnImmediately();

			EventManager::GetInstance()->SendEvent(eEventType::DeleteGameObject, GROUP_TYPE::ITEM, this);
		}
	}

	__super::Update(deltaTime);
}

void TrapItem::OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{
	if(!m_Active)
	{
		return;
	}

	m_pAttackedPlayer = dynamic_cast<PlayerObject*>(otherCollision->GetOwner());
	if (!m_pAttackedPlayer || otherCollision != m_pAttackedPlayer->GetBoxColliderPart())
	{
		return;
	}

	// 효과 발동
	m_bAffected = true;

	// 돈 드랍, 떨어지는 연출
	m_pAttackedPlayer->GetDamaged(0, m_pAttackedPlayer->GetMoney());
	m_pAttackedPlayer->FallOff();
	m_pAttackedPlayer->SetGameObjectLocation(GetGameObjectLocation().x, GetGameObjectLocation().y);

	m_pAttackedPlayer->SetVibration(0.2f, MaxVibrationValue / 2, MaxVibrationValue / 2);

	thisCollision->TurnOff_IsTrigger(otherCollision);

	iter->second = false;

	otherCollision->OnTriggerExit(thisCollision);
	thisCollision->OnTriggerExit(otherCollision);

	m_pCollider->SetCollisionType(COLLISION_TYPE::NONE);

	int playerNumber = m_pAttackedPlayer->GetPlayerNumber();

	if (playerNumber == 0)
		SoundManager::GetInstance()->PlayMusic(eSoundList::HoleEffect, eSoundChannel::Player1State, 1.f);
	else if (playerNumber == 1)																		
		SoundManager::GetInstance()->PlayMusic(eSoundList::HoleEffect, eSoundChannel::Player2State, 1.f);
	else if (playerNumber == 2)																		
		SoundManager::GetInstance()->PlayMusic(eSoundList::HoleEffect, eSoundChannel::Player3State, 1.f);
	else if (playerNumber == 3)																		
		SoundManager::GetInstance()->PlayMusic(eSoundList::HoleEffect, eSoundChannel::Player4State, 1.f);
}