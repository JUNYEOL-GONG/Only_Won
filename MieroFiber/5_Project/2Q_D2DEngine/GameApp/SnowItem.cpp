#include "framework.h"
#include "SnowItem.h"

#include "PlayerObject.h"
#include "../Engine/AnimationComponent.h"
#include "../Engine/PlayerMovement.h"
#include "../Engine/TextureComponent.h"

#include "../Engine/BoxCollider2D.h"
#include "../Engine/EventManager.h"
#include "../Engine/SoundManager.h"

SnowItem::~SnowItem()
{
}

bool SnowItem::Initialize()
{
	m_Damage = 2;
	m_DropedMoney = 1;

	/// TextureComponent
	m_pTextureComponent = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pTextureComponent->SetTextureAsset(L"../Resource/Snowball_2.png", L"SnowBall");
	m_pTextureComponent->SetRelativeLocation(m_Position.x, m_Position.y);
	m_pTextureComponent->SetRelativeRotation(m_Rotation);
	SetRootComponent(m_pTextureComponent);

	m_pEffectAnimation = CreateComponent<AnimationComponent>(L"EffectAnimation");
	std::vector<framework::FRAME_INFO> effectFrame;
	effectFrame.emplace_back(framework::FRAME_INFO{ 0, 0, 160, 160, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 160, 0, 160, 160, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 320, 0, 160, 160, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 480, 0, 160, 160, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 640, 0, 160, 160, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 0, 0, 0, 0, 0.1f });
	m_pEffectAnimation->SetAnimationAsset(L"../Resource/Effect_Snowball.png", L"SnowballEffect", effectFrame);
	m_pEffectAnimation->SetAnimationName(L"SnowballEffect");
	m_pEffectAnimation->SetLoop(false);
	m_pEffectAnimation->SetActive(false);
	m_pEffectAnimation->AttachToComponent(m_pTextureComponent);

	const bool res = ProjectileObject::Initialize();
	assert(res);

	return true;
}

void SnowItem::Update(float deltaTime)
{
	// 아이템 효과 발생 중이면
	if (m_IsAffected)
	{
		m_SlowTimer += deltaTime;

		// 슬로우 종료
		if (m_SlowTimer >= m_SlowTime)
		{
			m_SlowTimer = 0.f;
			m_IsAffected = false;

			// 슬로우 되돌리기
			m_PlayerMovement->SetOriginalVelocityLimit(1 / m_SlowRate);

			// this 총알 삭제
			EventManager::GetInstance()->SendEvent(eEventType::DeleteGameObject, GROUP_TYPE::ITEM, this);
		}
		else if(dynamic_cast<PlayerObject*>(m_pOwnerPlayer)->IsStun())
		{
			// 지속시간 끝나기 전에 스턴 상태가 되면
			// 슬로우 되돌리기
			m_PlayerMovement->SetOriginalVelocityLimit(1 / m_SlowRate);
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

void SnowItem::OnCollisionEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{

}

void SnowItem::OnCollisionStay(Collider2D* otherCollision)
{
	ProjectileObject::OnCollisionStay(otherCollision);
}

void SnowItem::OnCollisionExit(Collider2D* otherCollision)
{
	ProjectileObject::OnCollisionExit(otherCollision);
}

void SnowItem::OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{
	// 아이템 소유자와 충돌체의 소유자를 비교
	const auto targetObject = otherCollision->GetOwner();
	const auto ownerObject = this->GetOwnerPlayer();

	// 타겟의 플레이어 무브먼트
	PlayerObject* targetPlayer = dynamic_cast<PlayerObject*>(targetObject);
	m_PlayerMovement = targetObject->GetComponent<PlayerMovement>();

	// 충돌체와 부딪히면 오브젝트 삭제 (자신 제외)
	if(otherCollision->GetCollisionType() == COLLISION_TYPE::COLLISION && (targetObject != ownerObject))
	{
		// 플레이어인 경우 SnowItem의 피격 효과 발생
		if (targetPlayer && m_PlayerMovement) //&& !targetPlayer->IsStun())
		{
			// Damage
			dynamic_cast<PlayerObject*>(targetObject)->GetDamaged(m_Damage, m_DropedMoney);

			// 플레이어 느려짐 처리
			m_PlayerMovement->SetOriginalVelocityLimit(m_SlowRate);
			m_IsAffected = true;

			// 피격 플레이어는 진동을 느낀다
			targetPlayer->SetVibration(0.2f, MaxVibrationValue / 2, MaxVibrationValue / 2);

			// Slow효과 재생
			targetPlayer->PlaySlowEffect();

			int playerNumber = targetPlayer->GetPlayerNumber();

			if (playerNumber == 0)
				SoundManager::GetInstance()->PlayMusic(eSoundList::SnowballEffect, eSoundChannel::Player1State, 1.f);
			else if (playerNumber == 1)
				SoundManager::GetInstance()->PlayMusic(eSoundList::SnowballEffect, eSoundChannel::Player2State, 1.f);
			else if (playerNumber == 2)
				SoundManager::GetInstance()->PlayMusic(eSoundList::SnowballEffect, eSoundChannel::Player3State, 1.f);
			else if (playerNumber == 3)
				SoundManager::GetInstance()->PlayMusic(eSoundList::SnowballEffect, eSoundChannel::Player4State, 1.f);
		}

		// 대상 플레이어로 위치, 회전값 고정
		m_bTriggered = true;
		m_pTextureComponent->SetRelativeLocation(targetObject->GetGameObjectLocation().x, targetObject->GetGameObjectLocation().y);
		m_pTextureComponent->SetRelativeRotation(0.f);

		// 이펙트 애니메이션 재생
		m_pEffectAnimation->SetActive(true);

		// 콜라이더 NONE 처리
		m_pBoxCollider2D->SetCollisionType(COLLISION_TYPE::NONE);

		thisCollision->TurnOff_IsTrigger(otherCollision);

		iter->second = false;

		otherCollision->OnTriggerExit(thisCollision);
		thisCollision->OnTriggerExit(otherCollision);

		m_pTextureComponent->SetVisible(false);
		SoundManager::GetInstance()->PlayMusic(eSoundList::SnowballEffect, eSoundChannel::MapState, 1.f);
	}
}

void SnowItem::OnTriggerStay(Collider2D* otherCollision)
{
	ProjectileObject::OnTriggerStay(otherCollision);
}

void SnowItem::OnTriggerExit(Collider2D* otherCollision)
{
	ProjectileObject::OnTriggerExit(otherCollision);
}