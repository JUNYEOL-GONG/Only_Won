#include "framework.h"
#include "ObstacleObject.h"

#include "../Engine/TextureComponent.h"
#include "../Engine/BoxCollider2D.h"

ObstacleObject::ObstacleObject()
{
}

ObstacleObject::~ObstacleObject()
{
}

bool ObstacleObject::Initialize()
{
	/// TextureComponent
	m_pTextureComponent = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pTextureComponent->SetTextureAsset(L"../Resource/clear.png", L"Clear");
	SetRootComponent(m_pTextureComponent);

	/// BoxCollider2D
	m_pBoxCollider2D = CreateComponent<BoxCollider2D>(L"BoxCollider2D");
	m_pBoxCollider2D->AttachToComponent(m_pTextureComponent);

	const bool res = GameObject::Initialize();
	assert(res);

	return true;
}

void ObstacleObject::Update(float deltaTime)
{
	// 컴포넌트 업데이트
	GameObject::Update(deltaTime);
}

void ObstacleObject::OnCollisionEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{

}

void ObstacleObject::OnCollisionStay(Collider2D* otherCollision)
{

}

void ObstacleObject::OnCollisionExit(Collider2D* otherCollision)
{

}

void ObstacleObject::OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{

}

void ObstacleObject::OnTriggerStay(Collider2D* otherCollision)
{

}

void ObstacleObject::OnTriggerExit(Collider2D* otherCollision)
{

}

void ObstacleObject::SetTexture(const WCHAR* szFilePath, const std::wstring textureName) const
{
	m_pTextureComponent->SetTextureAsset(szFilePath, textureName);
}

void ObstacleObject::SetObstaclePos(float x, float y) const
{
	m_pTextureComponent->SetRelativeLocation(x, y);
}

void ObstacleObject::SetObstacleRotation(float angle) const
{
	m_pTextureComponent->SetRelativeRotation(angle);
}

void ObstacleObject::SetColliderPos(float x, float y) const
{
	m_pBoxCollider2D->SetRelativeLocation(x, y);
}

void ObstacleObject::SetColliderExtend(float x, float y) const
{
	m_pBoxCollider2D->SetExtend(x, y);
}