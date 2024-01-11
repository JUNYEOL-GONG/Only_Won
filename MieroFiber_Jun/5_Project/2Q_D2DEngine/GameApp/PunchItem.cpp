#include "framework.h"
#include "PunchItem.h"

#include "PlayerObject.h"
#include "../Engine/AnimationComponent.h"
#include "../Engine/PlayerMovement.h"
#include "../Engine/TextureComponent.h"

#include "../Engine/BoxCollider2D.h"
#include "../Engine/EventManager.h"
#include "../Engine/SoundManager.h"

PunchItem::~PunchItem()
{
}

bool PunchItem::Initialize()
{
	m_Damage = 2;
	m_DropedMoney = 1;

	/// TextureComponent
	m_pTextureComponent = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pTextureComponent->SetTextureAsset(L"../Resource/punch.png", L"Punch");
	m_pTextureComponent->SetRelativeLocation(m_Position.x, m_Position.y);
	m_pTextureComponent->SetRelativeRotation(m_Rotation);
	SetRootComponent(m_pTextureComponent);

	m_pEffectAnimation = CreateComponent<AnimationComponent>(L"EffectAnimation");
	std::vector<framework::FRAME_INFO> effectFrame;
	effectFrame.emplace_back(framework::FRAME_INFO{ 0,0,160,160,0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 160,0,160,160,0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 320,0,160,160,0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 480,0,160,160,0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 0, 0, 0, 0, 0.1f });
	m_pEffectAnimation->SetAnimationAsset(L"../Resource/Effect_Punch.png", L"PunchEffect", effectFrame);
	m_pEffectAnimation->SetAnimationName(L"PunchEffect");
	m_pEffectAnimation->SetLoop(false);
	m_pEffectAnimation->SetActive(false);
	m_pEffectAnimation->AttachToComponent(m_pTextureComponent);

	const bool res = ProjectileObject::Initialize();
	assert(res);

	return true;
}

void PunchItem::Update(float deltaTime)
{
	ProjectileObject::Update(deltaTime);
}

void PunchItem::OnCollisionEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{

}

void PunchItem::OnCollisionStay(Collider2D* otherCollision)
{
	ProjectileObject::OnCollisionStay(otherCollision);
}

void PunchItem::OnCollisionExit(Collider2D* otherCollision)
{
	ProjectileObject::OnCollisionExit(otherCollision);
}

void PunchItem::OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{
	const auto targetObject = otherCollision->GetOwner();	// 충돌한 대상
	const auto ownerObject = this->GetOwnerPlayer();		// 아이템 소유자

	PlayerObject* targetPlayer = dynamic_cast<PlayerObject*>(targetObject);	// 충돌한 대상이 플레이어인지 확인

	// 충돌한 대상이 플레이어이고, 스턴 상태가 아닌 경우 PunchItem의 피격 효과 발생
	if (targetPlayer && targetObject->GetComponent<PlayerMovement>()
		&& targetObject != ownerObject)// && !targetPlayer->IsStun())
	{
		// 닿은 순간 콜라이더 None
		m_pBoxCollider2D->SetCollisionType(COLLISION_TYPE::NONE);

		thisCollision->TurnOff_IsTrigger(otherCollision);

		iter->second = false;

		otherCollision->OnTriggerExit(thisCollision);
		thisCollision->OnTriggerExit(otherCollision);

		// 피격 플레이어는 진동을 느낀다
		targetPlayer->SetVibration(0.2f, MaxVibrationValue / 2, MaxVibrationValue / 2);

		// Effect : 대상 플레이어로 위치, 회전값 고정
		m_bTriggered = true;
		m_pTextureComponent->SetRelativeLocation(targetObject->GetGameObjectLocation().x, targetObject->GetGameObjectLocation().y);
		m_pTextureComponent->SetRelativeRotation(0.f);

		// 이펙트 애니메이션 재생
		m_pTextureComponent->SetVisible(false);
		m_pEffectAnimation->SetActive(true);

		// 넉백 방향 계산
		const framework::Vector2D targetPos = targetObject->GetRootComponent()->GetWorldLocation();
		const framework::Vector2D ownerObjectPos = ownerObject->GetRootComponent()->GetWorldLocation();
		const framework::Vector2D knockBackDir = (targetPos - ownerObjectPos).Normalize();

		dynamic_cast<PlayerObject*>(targetObject)->GetDamaged(m_Damage, m_DropedMoney);

		// 플레이어 넉백 처리
		otherCollision->GetOwner()->GetComponent<PlayerMovement>()->KnockBack(knockBackDir, knockBackPower);

		if (targetPlayer->GetPlayerNumber() == 0)
			SoundManager::GetInstance()->PlayMusic(eSoundList::PunchEffect, eSoundChannel::Player1State, 1.f);
		else if (targetPlayer->GetPlayerNumber() == 1)													 
			SoundManager::GetInstance()->PlayMusic(eSoundList::PunchEffect, eSoundChannel::Player2State, 1.f);
		else if (targetPlayer->GetPlayerNumber() == 2)													 
			SoundManager::GetInstance()->PlayMusic(eSoundList::PunchEffect, eSoundChannel::Player3State, 1.f);
		else if (targetPlayer->GetPlayerNumber() == 3)													 
			SoundManager::GetInstance()->PlayMusic(eSoundList::PunchEffect, eSoundChannel::Player4State, 1.f);
	}
}

void PunchItem::OnTriggerStay(Collider2D* otherCollision)
{
	ProjectileObject::OnTriggerStay(otherCollision);
}

void PunchItem::OnTriggerExit(Collider2D* otherCollision)
{
	ProjectileObject::OnTriggerExit(otherCollision);
}