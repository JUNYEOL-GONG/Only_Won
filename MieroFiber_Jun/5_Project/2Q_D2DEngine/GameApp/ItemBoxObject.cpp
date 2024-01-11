#include "framework.h"
#include "ItemBoxObject.h"

#include "ItemObject.h"

#include "../Engine/BoxCollider2D.h"
#include "../Engine/TextureComponent.h"
#include "../Engine/World.h"

bool ItemBoxObject::Initialize()
{
	m_pTextureComponent = CreateComponent<TextureComponent>(L"MoneyTexture");
	if(m_ItemBoxType == eItemBoxType::THROW)
	{
		m_pTextureComponent->SetTextureAsset(L"../Resource/item_box_throw.png", L"ItemBox_THROW");
	}
	else if (m_ItemBoxType == eItemBoxType::INSTALLATION)
	{
		m_pTextureComponent->SetTextureAsset(L"../Resource/item_box_install.png", L"ItemBox_INSTALL");
	}
	else if (m_ItemBoxType == eItemBoxType::REINFORCED)
	{
		m_pTextureComponent->SetTextureAsset(L"../Resource/item_box_reinforced.png", L"ItemBox_REINFORCED");
	}
	SetRootComponent(m_pTextureComponent);

	m_pBoxCollider = CreateComponent<BoxCollider2D>(L"BoxCollider2D");
	m_pBoxCollider->SetExtend(40.f, 40.f);
	m_pBoxCollider->SetCollisionType(COLLISION_TYPE::TRIGGER);
	m_pBoxCollider->AttachToComponent(m_pTextureComponent);

	bool res = __super::Initialize();
	assert(res);

	return true;
}

void ItemBoxObject::Update(const float deltaTime)
{
	GameObject::Update(deltaTime);
}