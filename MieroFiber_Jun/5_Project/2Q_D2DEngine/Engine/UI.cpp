#include "pch.h"
#include "UI.h"

#include "EventManager.h"
#include "InputManager.h"

bool UI::Initialize()
{
	bool res = __super::Initialize();
	assert(res);

	return true;
}

void UI::Update(const float deltaTime)
{
	CheckMouseOn();
	if(m_bMouseOn)
	{
		// printf("UI 안에 있음\n");
	}
}

void UI::Render(ID2D1RenderTarget* pRenderTarget)
{
}


void UI::CheckMouseOn()
{
	/*const float mouseX = InputManager::GetInstance()->GetPadAxisLeftThumb(0).x;
	const float mouseY = InputManager::GetInstance()->GetPadAxisLeftThumb(0).y;

	if (mouseX > m_finalUIPos.x - (m_Width / 2) && mouseX < m_finalUIPos.x + (m_Width / 2)
		&& mouseY > m_finalUIPos.y - (m_Height / 2) && mouseY < (m_Height / 2) + m_finalUIPos.y)
	{
		m_bMouseOn = true;
		return;
	}
	m_bMouseOn = false;*/
}