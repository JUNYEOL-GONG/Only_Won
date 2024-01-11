#include "framework.h"
#include "PopUpUIObject2.h"


#include "../Engine/ButtonUI.h"
#include "../Engine/PopUpUI.h"

bool PopUpUIObject2::Initialize()
{
	m_pPopUpPanel = CreateComponent<PopUpUI>(L"PopUpUI");
	m_pPopUpPanel->SetWidth(300.f);
	m_pPopUpPanel->SetHeight(100.f);
	m_pPopUpPanel->SetColor(D2D1::ColorF::White);
	m_pPopUpPanel->SetRelativeLocation(ScreenWidth / 2.f, ScreenHeight / 2.f);
	SetRootComponent(m_pPopUpPanel);

	m_pOkayButton = CreateComponent<ButtonUI>(L"OkayButton");
	m_pOkayButton->SetWidth(50.f);
	m_pOkayButton->SetHeight(50.f);
	m_pOkayButton->SetColor(D2D1::ColorF::Red);
	m_pOkayButton->SetRelativeLocation(-50.f, 0.f);
	m_pOkayButton->SetYIndex(0);
	m_pOkayButton->SetXIndex(0);
	//m_pOkayButton->SetButtonEventType(eEventType::Okay);
	m_pOkayButton->AttachToComponent(m_pPopUpPanel);

	m_pCancelPanel = CreateComponent<ButtonUI>(L"NoButton");
	m_pCancelPanel->SetWidth(50.f);
	m_pCancelPanel->SetHeight(50.f);
	m_pCancelPanel->SetColor(D2D1::ColorF::Green);
	m_pCancelPanel->SetRelativeLocation(50.f, 0.f);
	m_pCancelPanel->SetYIndex(0);
	m_pCancelPanel->SetXIndex(1);
	//m_pCancelPanel->SetButtonEventType(eEventType::No);
	m_pCancelPanel->AttachToComponent(m_pPopUpPanel);

	__super::Initialize();

	return true;
}

void PopUpUIObject2::Update(const float deltaTime)
{
}