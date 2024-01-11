#include "framework.h"
#include "SnareItem.h"

#include "PlayerObject.h"
#include "../Engine/AnimationComponent.h"
#include "../Engine/BoxCollider2D.h"
#include "../Engine/EventManager.h"
#include "../Engine/PlayerMovement.h"
#include "../Engine/TextureComponent.h"
#include "../Engine/SoundManager.h"

bool SnareItem::Initialize()
{
	m_Duration = 5.f;
	m_Damage = 2;
	m_DropedMoney = 2;

	m_pTextureComponent = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pTextureComponent->SetTextureAsset(L"../Resource/덫_설치.png", L"SnareItem");
	m_pTextureComponent->SetRelativeLocation(m_pOwnerPlayer->GetGameObjectLocation().x, m_pOwnerPlayer->GetGameObjectLocation().y);
	SetRootComponent(m_pTextureComponent);

	m_pCollider = CreateComponent<BoxCollider2D>(L"Collider2D");
	m_pCollider->SetCollisionType(COLLISION_TYPE::TRIGGER);
	m_pCollider->SetExtend(35.f, 20.f);
	m_pCollider->AttachToComponent(m_pTextureComponent);

	m_pEffectAnimation = CreateComponent<AnimationComponent>(L"EffectAnimation");
	std::vector<framework::FRAME_INFO> effectFrame;
	effectFrame.emplace_back(framework::FRAME_INFO{ 0, 0, 130, 70, 0.1f});
	effectFrame.emplace_back(framework::FRAME_INFO{ 130, 0, 130, 70, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 260, 0, 130, 70, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 390, 0, 130, 70, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 520, 0, 130, 70, 0.3f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 0, 0, 0, 0, 0.1f });	// 마지막프레임 제대로 재생하기 위한 빈 프레임 정보
	m_pEffectAnimation->SetAnimationAsset(L"../Resource/Effect_Snare.png", L"SnareEffect", effectFrame);
	m_pEffectAnimation->SetLoop(false);
	m_pEffectAnimation->SetActive(false);
	m_pEffectAnimation->AttachToComponent(m_pTextureComponent);

	bool res = __super::Initialize();
	assert(res);

	return true;
}

void SnareItem::Update(const float deltaTime)
{
	// 설치 후 1초간 효력 발동 X
	m_DelayTime += deltaTime;
	if (m_DelayTime >= 1.f)
	{
		m_Active = true;
	}

	if(m_bAffected)
	{
		// 지속시간 끝난 후
		m_ProgressTime += deltaTime;
		if(m_ProgressTime >= m_Duration)
		{
			m_pPlayerMovement->SetOriginalVelocityLimit(2.f);
			m_DelayTime = 0.f;
			m_ProgressTime = 0.f;

			EventManager::GetInstance()->SendEvent(eEventType::DeleteGameObject, GROUP_TYPE::ITEM, this);
		}
		else if(dynamic_cast<PlayerObject*>(m_pOwnerPlayer)->IsStun())
		{
			// 지속시간 끝나기 전에 스턴 상태가 되면
			m_pPlayerMovement->SetOriginalVelocityLimit(2.f);
			EventManager::GetInstance()->SendEvent(eEventType::DeleteGameObject, GROUP_TYPE::ITEM, this);
		}
	}

	__super::Update(deltaTime);
}

void SnareItem::OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{
	if(!m_Active)
	{
		return;
	}

	m_pPlayer = dynamic_cast<PlayerObject*>(otherCollision->GetOwner());
	if(!m_pPlayer || otherCollision != m_pPlayer->GetBoxColliderPart())
	{
		return;
	}

	// 효과 발동
	m_bAffected = true;

	m_pPlayer->GetDamaged(m_Damage, m_DropedMoney);
	m_pPlayerMovement = m_pPlayer->GetComponent<PlayerMovement>();
	m_pPlayerMovement->SetOriginalVelocityLimit(0.5f);

	m_pPlayer->SetVibration(0.2f, MaxVibrationValue / 2, MaxVibrationValue / 2);

	// Blood 이펙트 재생
	m_pPlayer->PlayBloodEffect();

	thisCollision->TurnOff_IsTrigger(otherCollision);

	iter->second = false;

	otherCollision->OnTriggerExit(thisCollision);
	thisCollision->OnTriggerExit(otherCollision);

	m_pCollider->SetCollisionType(COLLISION_TYPE::NONE);

	// Effect 재생
	m_pTextureComponent->SetVisible(false);
	m_pEffectAnimation->SetAnimationName(L"SnareEffect");
	m_pEffectAnimation->SetActive(true);

	int playerNumber = m_pPlayer->GetPlayerNumber();

	if(playerNumber == 0)
		SoundManager::GetInstance()->PlayMusic(eSoundList::TrapEffect, eSoundChannel::Player1State, 1.f);
	else if (playerNumber == 1)																		
		SoundManager::GetInstance()->PlayMusic(eSoundList::TrapEffect, eSoundChannel::Player2State, 1.f);
	else if (playerNumber == 2)																		
		SoundManager::GetInstance()->PlayMusic(eSoundList::TrapEffect, eSoundChannel::Player3State, 1.f);
	else if (playerNumber == 3)																		
		SoundManager::GetInstance()->PlayMusic(eSoundList::TrapEffect, eSoundChannel::Player4State, 1.f);
}