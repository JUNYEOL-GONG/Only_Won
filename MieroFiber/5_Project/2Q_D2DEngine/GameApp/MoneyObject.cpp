#include "framework.h"
#include "MoneyObject.h"

#include "../Engine/BoxCollider2D.h"
#include "../Engine/EventManager.h"
#include "../Engine/TextureComponent.h"
#include "../Engine/World.h"

bool MoneyObject::Initialize()
{
	m_pTextureComponent = CreateComponent<TextureComponent>(L"MoneyTexture");
	if(m_MoneyType == eMoneyType::SPAWNED)
	{
		m_pTextureComponent->SetTextureAsset(L"../Resource/MoneyObject.png", L"MoneyObject");
	}
	else
	{
		m_pTextureComponent->SetTextureAsset(L"../Resource/MoneyObjectBig.png", L"MoneyObject");
	}
	SetRootComponent(m_pTextureComponent);

	m_pCollider = CreateComponent<BoxCollider2D>(L"BoxCollider2D");
	if (m_MoneyType == eMoneyType::SPAWNED)
	{
		m_pCollider->SetExtend(25.f, 25.f);
	}
	else
	{
		m_pCollider->SetExtend(40.f, 40.f);
	}
	m_pCollider->SetCollisionType(COLLISION_TYPE::TRIGGER);

	m_pCollider->AttachToComponent(m_pTextureComponent);

	bool res = __super::Initialize();
	assert(res);

	return true;
}

void MoneyObject::Update(const float deltaTime)
{
	if (this->m_MoneyType == eMoneyType::DROPED_UNCOLLISION)
	{
		// 1초 뒤에 먹을 수 있는 돈으로 변경
		m_ElapsedTime += deltaTime;
		if (m_ElapsedTime >= m_DelayTime)
		{
			m_MoneyType = eMoneyType::DROPED_COLLISION;
		}
	}


	__super::Update(deltaTime);
}