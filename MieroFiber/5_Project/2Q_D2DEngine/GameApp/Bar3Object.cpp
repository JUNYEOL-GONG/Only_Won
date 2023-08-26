#include "framework.h"
#include "Bar3Object.h"

#include "InGameWorld.h"
#include "PlayerObject.h"
#include "../Engine/BarUI.h"
#include "../Engine/TextureComponent.h"

bool Bar3Object::Initialize()
{
	float m_scaleX = -1.f;
	m_prevMoney = 0;
	m_goalMoney = dynamic_cast<InGameWorld*>(this->GetOwnerWorld())->GetGoalScore();
	m_barRate = 2.f / (m_goalMoney / m_moneyBundle);

	m_pBarUI = CreateComponent<BarUI>(L"BarUI");
	m_pBarUI->SetWidth(1082);
	m_pBarUI->SetHeight(13);
	m_pBarUI->SetColor(D2D1::ColorF::White);
	m_pBarUI->SetRelativeLocation(960.f, 65.f);
	SetRootComponent(m_pBarUI);

	m_pBarTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pBarTexture->SetTextureAsset(L"../Resource/graph_yellow.png", L"");
	m_pBarTexture->SetRelativeLocation(0.f, 0.f);
	m_pBarTexture->SetScale(-1.f, 1.f);
	m_pBarTexture->AttachToComponent(m_pBarUI);

	m_pBarFlagTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pBarFlagTexture->SetTextureAsset(L"../Resource/graph_flag_yellow.png", L"");
	m_pBarFlagTexture->SetRelativeLocation(-550.f, 0.f);
	m_pBarFlagTexture->AttachToComponent(m_pBarUI);

	__super::Initialize();

	return true;
}

void Bar3Object::Update(const float deltaTime)
{
	// µ·¹¶Ä¡ ¸î°³ ³Ö¾ú´ÂÁö
	int countMoney = 0;
	if (m_prevMoney >= (m_goalMoney / m_moneyBundle))
	{
		// m_scaleX = -1.f;
		return;
	}

	if (m_prevMoney != dynamic_cast<PlayerObject*>(this->GetOwnerObj())->GetScore())
	{
		if (dynamic_cast<PlayerObject*>(this->GetOwnerObj())->GetScore() >= (m_goalMoney / m_moneyBundle))
		{
			m_pBarFlagTexture->SetRelativeLocation(550.f, 0.f);
			m_pBarTexture->SetScale(1.f, 1.f);
			m_prevMoney = dynamic_cast<PlayerObject*>(this->GetOwnerObj())->GetScore();
			__super::Update(deltaTime);
			return;
		}
		countMoney = (dynamic_cast<PlayerObject*>(this->GetOwnerObj())->GetScore() - m_prevMoney);
		m_relativeX += (m_pBarUI->GetWidth() / (m_goalMoney / m_moneyBundle)) * countMoney;
		m_scaleX += (m_barRate)*countMoney;
		m_pBarFlagTexture->SetRelativeLocation(m_relativeX, 0.f);
		m_pBarTexture->SetScale(m_scaleX, 1.f);
		m_prevMoney = dynamic_cast<PlayerObject*>(this->GetOwnerObj())->GetScore();
	}
	__super::Update(deltaTime);
}
