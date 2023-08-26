#include "framework.h"
#include "BottleItem.h"

#include "PlayerObject.h"
#include "../Engine/AnimationComponent.h"
#include "../Engine/PlayerMovement.h"
#include "../Engine/TextureComponent.h"

#include "../Engine/BoxCollider2D.h"
#include "../Engine/EventManager.h"
#include "../Engine/SoundManager.h"

BottleItem::~BottleItem()
{
}

bool BottleItem::Initialize()
{
	m_Damage = 2;
	m_DropedMoney = 1;

	/// TextureComponent
	m_pTextureComponent = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pTextureComponent->SetTextureAsset(L"../Resource/glassBottle.png", L"GlassBottle");
	m_pTextureComponent->SetRelativeLocation(m_Position.x, m_Position.y);
	m_pTextureComponent->SetRelativeRotation(m_Rotation);
	// m_pTextureComponent->SetRelativeScale(2.f, 2.f);
	SetRootComponent(m_pTextureComponent);

	m_pEffectAnimation = CreateComponent<AnimationComponent>(L"EffectAnimation");
	std::vector<framework::FRAME_INFO> effectFrame;
	effectFrame.emplace_back(framework::FRAME_INFO{0, 0, 160, 160, 0.1f});
	effectFrame.emplace_back(framework::FRAME_INFO{ 160, 0, 160, 160, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 320, 0, 160, 160, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 480, 0, 160, 160, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 640, 0, 160, 160, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 800, 0, 160, 160, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 960, 0, 160, 160, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1120, 0, 160, 160, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1280, 0, 160, 160, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1440, 0, 160, 160, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1600, 0, 160, 16, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1760, 0, 160, 160, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 0, 0, 0, 0, 0.1f });
	m_pEffectAnimation->SetAnimationAsset(L"../Resource/Effect_Bottle.png", L"BottleEffect", effectFrame);
	m_pEffectAnimation->SetAnimationName(L"BottleEffect");
	m_pEffectAnimation->SetLoop(false);
	m_pEffectAnimation->SetActive(false);
	m_pEffectAnimation->AttachToComponent(m_pTextureComponent);

	const bool res = ProjectileObject::Initialize();
	assert(res);

	return true;
}

void BottleItem::Update(float deltaTime)
{
	// 아이템 효과 발생 중이면
	if (m_IsAffected)
	{
		m_BottleTimer += deltaTime;

		// 슬로우 종료
		if (m_BottleTimer >= m_BottleTime)
		{
			m_BottleTimer = 0.f;
			m_IsAffected = false;

			// 플레이어 이동 반전 종료
			m_PlayerMovement->ReverseMove();

			// this 총알 삭제
			EventManager::GetInstance()->SendEvent(eEventType::DeleteGameObject, GROUP_TYPE::ITEM, this);
		}

		GameObject::Update(deltaTime);
	}
	else
	{
		// 날아가다가 자연스럽게 사라짐
		ProjectileObject::Update(deltaTime);
	}
}

void BottleItem::OnCollisionEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{

}

void BottleItem::OnCollisionStay(Collider2D* otherCollision)
{
	ProjectileObject::OnCollisionStay(otherCollision);
}

void BottleItem::OnCollisionExit(Collider2D* otherCollision)
{
	ProjectileObject::OnCollisionExit(otherCollision);
}

void BottleItem::OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{
	const auto targetObject = otherCollision->GetOwner();	// 충돌한 대상
	const auto ownerObject = this->GetOwnerPlayer();		// 아이템 소유자

	m_PlayerMovement = targetObject->GetComponent<PlayerMovement>();
	PlayerObject* targetPlayer = dynamic_cast<PlayerObject*>(targetObject);

	// 충돌한 대상이 플레이어인 경우 BottleItem의 피격 효과 발생
	if (targetPlayer && m_PlayerMovement
		&& targetObject != ownerObject)// && !targetPlayer->IsStun())
	{
		// 플레이어 이동 방향 반대로
		m_PlayerMovement->ReverseMove();
		m_IsAffected = true;

		// 피격 플레이어는 진동을 느낀다
		targetPlayer->SetVibration(0.2f, MaxVibrationValue / 2, MaxVibrationValue / 2);

		// Damage
		dynamic_cast<PlayerObject*>(targetObject)->GetDamaged(m_Damage, m_DropedMoney);

		// 닿은 순간 콜라이더 None
		m_pBoxCollider2D->SetCollisionType(COLLISION_TYPE::NONE);

		thisCollision->TurnOff_IsTrigger(otherCollision);

		iter->second = false;

		otherCollision->OnTriggerExit(thisCollision);
		thisCollision->OnTriggerExit(otherCollision);

		// Effect : 대상 플레이어로 위치, 회전값 고정
		m_bTriggered = true;
		m_pTextureComponent->SetRelativeLocation(targetObject->GetGameObjectLocation().x, targetObject->GetGameObjectLocation().y);
		m_pTextureComponent->SetRelativeRotation(0.f);

		// 이펙트 애니메이션 재생
		m_pTextureComponent->SetVisible(false);
		m_pEffectAnimation->SetActive(true);

		// 대상 플레이어 negative effect 재생
		targetPlayer->PlayNegativeEffect();

		if(targetPlayer->GetPlayerNumber() == 0)
			SoundManager::GetInstance()->PlayMusic(eSoundList::BottleEffect, eSoundChannel::Player1State, 1.f);
		else if (targetPlayer->GetPlayerNumber() == 1)													  
			SoundManager::GetInstance()->PlayMusic(eSoundList::BottleEffect, eSoundChannel::Player2State, 1.f);
		else if (targetPlayer->GetPlayerNumber() == 2)													  
			SoundManager::GetInstance()->PlayMusic(eSoundList::BottleEffect, eSoundChannel::Player3State, 1.f);
		else if (targetPlayer->GetPlayerNumber() == 3)													  
			SoundManager::GetInstance()->PlayMusic(eSoundList::BottleEffect, eSoundChannel::Player4State, 1.f);
	}
}

void BottleItem::OnTriggerStay(Collider2D* otherCollision)
{
	ProjectileObject::OnTriggerStay(otherCollision);
}

void BottleItem::OnTriggerExit(Collider2D* otherCollision)
{
	ProjectileObject::OnTriggerExit(otherCollision);
}