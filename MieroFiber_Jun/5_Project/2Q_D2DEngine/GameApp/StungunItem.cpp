#include "framework.h"
#include "StungunItem.h"

#include "PlayerObject.h"
#include "../Engine/AnimationComponent.h"
#include "../Engine/PlayerMovement.h"
#include "../Engine/TextureComponent.h"

#include "../Engine/BoxCollider2D.h"
#include "../Engine/EventManager.h"
#include "../Engine/SoundManager.h"

StungunItem::~StungunItem()
{
}

bool StungunItem::Initialize()
{
	m_Damage = 5;
	m_DropedMoney = 2;

	/// TextureComponent
	m_pTextureComponent = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pTextureComponent->SetTextureAsset(L"../Resource/stunGun.png", L"StunGun");
	m_pTextureComponent->SetRelativeLocation(m_Position.x, m_Position.y);
	m_pTextureComponent->SetRelativeRotation(m_Rotation);
	m_pTextureComponent->SetVisible(false);
	SetRootComponent(m_pTextureComponent);

	m_pEffectAnimation = CreateComponent<AnimationComponent>(L"EffectAnimation");
	std::vector<framework::FRAME_INFO> effectFrame;
	effectFrame.emplace_back(framework::FRAME_INFO{ 0, 0, 200, 100, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 200, 0, 200, 100, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 400, 0, 200, 100, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 600, 0, 200, 100, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 800, 0, 200, 100, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1000, 0, 200, 100, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1200, 0, 200, 100, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1400, 0, 200, 100, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1600, 0, 200, 100, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1800, 0, 200, 100, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2000, 0, 200, 100, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2200, 0, 200, 100, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2400, 0, 200, 100, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2600, 0, 200, 100, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2800, 0, 200, 100, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 3000, 0, 200, 100, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 0, 0, 0, 0, 0.1f });
	m_pEffectAnimation->SetAnimationAsset(L"../Resource/Effect_Stungun.png", L"StungunEffect", effectFrame);
	m_pEffectAnimation->SetAnimationName(L"StungunEffect");
	m_pEffectAnimation->SetLoop(false);
	m_pEffectAnimation->SetActive(false);
	m_pEffectAnimation->AttachToComponent(m_pTextureComponent);

	const bool res = ProjectileObject::Initialize();
	assert(res);

	return true;
}

void StungunItem::Update(float deltaTime)
{
	// ���� �ð��� ����
	m_stunGunLifeTime += deltaTime;

	// �ڿ� ����
	if (m_stunGunLifeTime >= m_stunGunMaxLifeTime)
	{
		m_stunGunLifeTime = 0.f;

		// World�� m_GameObjects���� ��������� �Ѵ�
		EventManager::GetInstance()->SendEvent(eEventType::DeleteGameObject, GROUP_TYPE::ITEM, this);
		return;
	}

	// �ε����� ���� ��쿡�� �̵�
	if (!m_bTriggered)
	{
		// m_Direction�� m_Speed�� �̿��Ͽ� �̵�
		m_Velocity = m_Direction * m_Speed;

		// ����ü�� �̵�
		GetRootComponent()->AddRelativeLocation(m_Velocity * deltaTime);
	}

	// ��� ������Ʈ ������Ʈ
	GameObject::Update(deltaTime);
}

void StungunItem::OnCollisionEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{

}

void StungunItem::OnCollisionStay(Collider2D* otherCollision)
{
	ProjectileObject::OnCollisionStay(otherCollision);
}

void StungunItem::OnCollisionExit(Collider2D* otherCollision)
{
	ProjectileObject::OnCollisionExit(otherCollision);
}

void StungunItem::OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{
	const auto targetObject = otherCollision->GetOwner();	// �浹�� ���
	const auto ownerObject = this->GetOwnerPlayer();		// ������ ������

	PlayerObject* targetPlayer = dynamic_cast<PlayerObject*>(targetObject);

	// �浹�� ����� �÷��̾��� ��� BottleItem�� �ǰ� ȿ�� �߻�
	if (targetPlayer && targetObject != ownerObject)// && !targetPlayer->IsStun())
	{
		// Damage
		dynamic_cast<PlayerObject*>(targetObject)->GetDamaged(m_Damage, m_DropedMoney);

		// ���� ���� �ݶ��̴� None
		m_pBoxCollider2D->SetCollisionType(COLLISION_TYPE::NONE);

		thisCollision->TurnOff_IsTrigger(otherCollision);

		iter->second = false;

		otherCollision->OnTriggerExit(thisCollision);
		thisCollision->OnTriggerExit(otherCollision);

		// �ǰ� �÷��̾�� ������ ������
		targetPlayer->SetVibration(0.2f, MaxVibrationValue / 2, MaxVibrationValue / 2);

		// Effect ȸ�� + �̵� �� ���
		m_bTriggered = true;

		framework::Vector2D tempVector = targetObject->GetGameObjectLocation() - ownerObject->GetGameObjectLocation();
		framework::Vector2D normalizedVector = tempVector.GetNormalize();
		float theta = atan(normalizedVector.y / normalizedVector.x);
		theta = theta * 180.f / 3.14159265f;
		m_pTextureComponent->SetRelativeRotation(theta);
		m_pTextureComponent->AddRelativeLocation(-tempVector.x + normalizedVector.x * 150.f, -tempVector.y + normalizedVector.y * 150.f);

		// ����Ʈ �ִϸ��̼� ���
		m_pEffectAnimation->SetActive(true);

		SoundManager::GetInstance()->PlayMusic(eSoundList::WaveEffect, eSoundChannel::MapState, 1.f);
	}
}

void StungunItem::OnTriggerStay(Collider2D* otherCollision)
{
	ProjectileObject::OnTriggerStay(otherCollision);
}

void StungunItem::OnTriggerExit(Collider2D* otherCollision)
{
	ProjectileObject::OnTriggerExit(otherCollision);
}