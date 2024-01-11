#include "pch.h"
#include "PopUpUI.h"

#include "CommonApp.h"

bool PopUpUI::Initialize()
{
	bool res = __super::Initialize();
	assert(res);

	return true;
}

void PopUpUI::Update(const float deltaTime)
{
}

void PopUpUI::Render(ID2D1RenderTarget* pRenderTarget)
{
	CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);
	m_FinalTransform = m_WorldTransform * GetCameraTransform();
	pRenderTarget->SetTransform(m_FinalTransform);
	pRenderTarget->DrawRectangle(
		D2D1::RectF(-m_Width / 2, -m_Height / 2, m_Width / 2, m_Height / 2),
		CommonApp::m_pInstance->GetBrush(),
		2.f);
}