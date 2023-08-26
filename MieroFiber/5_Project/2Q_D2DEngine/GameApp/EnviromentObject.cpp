#include "framework.h"
#include "EnviromentObject.h"

#include "../Engine/TextureComponent.h"
#include "../Engine/BoxCollider2D.h"

EnviromentObject::EnviromentObject()
{
}

EnviromentObject::~EnviromentObject()
{
}

bool EnviromentObject::Initialize()
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

void EnviromentObject::Update(float deltaTime)
{
	// 컴포넌트 업데이트
	GameObject::Update(deltaTime);
}

void EnviromentObject::OnCollisionEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{

}

void EnviromentObject::OnCollisionStay(Collider2D* otherCollision)
{

}

void EnviromentObject::OnCollisionExit(Collider2D* otherCollision)
{

}

void EnviromentObject::OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{

}

void EnviromentObject::OnTriggerStay(Collider2D* otherCollision)
{

}

void EnviromentObject::OnTriggerExit(Collider2D* otherCollision)
{

}

void EnviromentObject::SetTexture(const WCHAR* szFilePath, const std::wstring textureName) const
{
	m_pTextureComponent->SetTextureAsset(szFilePath, textureName);
}

void EnviromentObject::SetObstaclePos(float x, float y) const
{
	m_pTextureComponent->SetRelativeLocation(x, y);
}

void EnviromentObject::SetObstacleRotation(float angle) const
{
	m_pTextureComponent->SetRelativeRotation(angle);
}

void EnviromentObject::SetColliderPos(float x, float y) const
{
	m_pBoxCollider2D->SetRelativeLocation(x, y);
}

void EnviromentObject::SetColliderExtend(float x, float y) const
{
	m_pBoxCollider2D->SetExtend(x, y);
}
