#include "framework.h"
#include "ProjectileObject.h"

#include "../Engine/TextureComponent.h"

#include "../Engine/BoxCollider2D.h"
#include "../Engine/EventManager.h"
#include "../Engine/PlayerMovement.h"

ProjectileObject::~ProjectileObject()
{
}

bool ProjectileObject::Initialize()
{
	/// BoxCollider2D
	m_pBoxCollider2D = CreateComponent<BoxCollider2D>(L"BoxCollider2D");
	m_pBoxCollider2D->SetCollisionType(COLLISION_TYPE::TRIGGER);
	m_pBoxCollider2D->SetExtend(20.f, 20.f);
	m_pBoxCollider2D->AttachToComponent(m_pTextureComponent);

	const bool res = GameObject::Initialize();
	assert(res);

	return true;
}

void ProjectileObject::Update(const float deltaTime)
{
	// 생존 시간을 증가
	m_LifeTime += deltaTime;

	// 자연 삭제
	if (m_LifeTime >= m_MaxLifeTime)
	{
		m_LifeTime = 0.f;

		// World의 m_GameObjects에서 제거해줘야 한다
		EventManager::GetInstance()->SendEvent(eEventType::DeleteGameObject, GROUP_TYPE::ITEM, this);
		return;
	}

	// 부딪히지 않은 경우에만 이동
	if (!m_bTriggered)
	{
		// m_Direction과 m_Speed를 이용하여 이동
		m_Velocity = m_Direction * m_Speed;

		// 투사체의 이동
		GetRootComponent()->AddRelativeLocation(m_Velocity * deltaTime);
	}

	// 모든 컴포넌트 업데이트
	GameObject::Update(deltaTime);
}

void ProjectileObject::OnCollisionEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{

}

void ProjectileObject::OnCollisionStay(Collider2D* otherCollision)
{

}

void ProjectileObject::OnCollisionExit(Collider2D* otherCollision)
{

}

void ProjectileObject::OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{

}

void ProjectileObject::OnTriggerStay(Collider2D* otherCollision)
{

}

void ProjectileObject::OnTriggerExit(Collider2D* otherCollision)
{

}