#include "pch.h"
#include "BarUI.h"

#include "CommonApp.h"
#include "TextureAsset.h"

bool BarUI::Initialize()
{
	bool res = __super::Initialize();
	assert(res);

	return true;
}

void BarUI::Update(const float deltaTime)
{
	__super::Update(deltaTime);
}

void BarUI::Render(ID2D1RenderTarget* pRenderTarget)
{
	//__super::Render(pRenderTarget);
	//m_FinalTransform = m_WorldTransform * GetCameraTransform();
	//pRenderTarget->SetTransform(m_FinalTransform);
	//pRenderTarget->DrawRectangle(
	//	D2D1::RectF(-m_Width / 2, -m_Height / 2, m_Width / 2, m_Height / 2),
	//	CommonApp::m_pInstance->GetBrush(),
	//	2.f);
	//pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}
