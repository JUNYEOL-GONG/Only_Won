#include "pch.h"
#include "PanelUI.h"

#include "CommonApp.h"
#include "Event.h"
#include "EventManager.h"
#include "TextureComponent.h"

bool PanelUI::Initialize()
{
	bool res = __super::Initialize();
	assert(res);

	return true;
}

void PanelUI::Update(const float deltaTime)
{
	SceneComponent::Update(deltaTime);

	if(!this->m_Children.empty())
	{
		//for(size_t i = 0; i < this->m_Children.size(); i++)
	}
}

void PanelUI::Render(ID2D1RenderTarget* pRenderTarget)
{
	//CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);
	//m_FinalTransform = m_WorldTransform * GetCameraTransform();
	//pRenderTarget->SetTransform(m_FinalTransform);

	//pRenderTarget->DrawRectangle(
	//	D2D1::RectF(-m_Width / 2, -m_Height / 2, m_Width / 2, m_Height / 2),
	//	CommonApp::m_pInstance->GetBrush(),
	//	2.f);

	//pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

