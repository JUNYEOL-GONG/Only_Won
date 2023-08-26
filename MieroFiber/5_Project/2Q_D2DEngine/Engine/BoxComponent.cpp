#include "pch.h"
#include "BoxComponent.h"

#include "CommonApp.h"

void BoxComponent::Render(ID2D1RenderTarget* pRenderTarget)
{
	m_FinalTransform = m_WorldTransform * GetCameraTransform();
	pRenderTarget->SetTransform(m_FinalTransform);

	SetColor(D2D1::ColorF::White);
	CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);

	pRenderTarget->DrawRectangle(
		D2D1::RectF(-m_Extend.x, -m_Extend.y, m_Extend.x, m_Extend.y),
		CommonApp::m_pInstance->GetBrush(),
		2.f);

	SetColor(D2D1::ColorF::Red);
	CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);

	// Draw Text Object Name
	pRenderTarget->DrawTextW(
		m_pOwner->GetName().c_str(),
		static_cast<UINT32>(m_pOwner->GetName().length()),
		CommonApp::m_pInstance->GetTextFormat(),
		D2D1::RectF(-50, -150, 50, -50),
		CommonApp::m_pInstance->GetBrush()
	);

	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}