#include "pch.h"
#include "ButtonUI.h"

#include "CommonApp.h"
#include "EventManager.h"
#include "InputManager.h"

bool ButtonUI::Initialize()
{
	bool res = __super::Initialize();
	assert(res);

	/*m_finalUIPos.x = this->GetWorldLocation().x;
	m_finalUIPos.y = this->GetWorldLocation().y;*/

	return true;
}

void ButtonUI::Update(const float deltaTime)
{
	__super::Update(deltaTime);
}

void ButtonUI::Render(ID2D1RenderTarget* pRenderTarget)
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

void ButtonUI::SetButtonEventType(eEventType eventType)
{
	m_buttonEventType = eventType;
}

