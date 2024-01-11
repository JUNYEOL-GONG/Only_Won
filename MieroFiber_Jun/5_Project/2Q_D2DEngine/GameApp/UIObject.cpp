#include "framework.h"
#include "UIObject.h"

#include "../Engine/ButtonUI.h"
#include "../Engine/PanelUI.h"
#include "../Engine/EventManager.h"

bool UIObject::Initialize()
{
	m_pStartPanel = CreateComponent<PanelUI>(L"StartPanel");
	m_pStartPanel->SetWidth(150.f);
	m_pStartPanel->SetHeight(500.f);
	m_pStartPanel->SetColor(D2D1::ColorF::White);
	m_pStartPanel->SetRelativeLocation(ScreenWidth / 4.f, ScreenHeight / 2.f);
	SetRootComponent(m_pStartPanel);

	m_pStartButton = CreateComponent<ButtonUI>(L"StartButton");
	m_pStartButton->SetWidth(50.f);
	m_pStartButton->SetHeight(50.f);
	m_pStartButton->SetColor(D2D1::ColorF::Red);
	m_pStartButton->SetRelativeLocation(-50.f, -100.f);
	m_pStartButton->SetYIndex(0);
	m_pStartButton->SetXIndex(0);
	m_pStartButton->SetButtonEventType(eEventType::ChangeWorld);
	m_pStartButton->SetActiveUI(false);
	m_pStartButton->AttachToComponent(m_pStartPanel);

	m_pMethodButton = CreateComponent<ButtonUI>(L"MethodButton");
	m_pMethodButton->SetWidth(50.f);
	m_pMethodButton->SetHeight(50.f);
	m_pMethodButton->SetColor(D2D1::ColorF::Green);
	m_pMethodButton->SetRelativeLocation(50.f, -100.f);
	/*m_pMethodButton->SetYIndex(0);
	m_pMethodButton->SetXIndex(2);*/
	m_pMethodButton->SetActiveUI(false);
	m_pMethodButton->AttachToComponent(m_pStartPanel);

	m_pCenterButton = CreateComponent<ButtonUI>(L"CenterButton");
	m_pCenterButton->SetWidth(50.f);
	m_pCenterButton->SetHeight(50.f);
	m_pCenterButton->SetColor(D2D1::ColorF::White);
	m_pCenterButton->SetRelativeLocation(0.f, 0.f);
	m_pCenterButton->SetYIndex(1);
	m_pCenterButton->SetXIndex(1);
	m_pCenterButton->SetActiveUI(false);
	m_pCenterButton->AttachToComponent(m_pStartPanel);

	m_pMadeByButton = CreateComponent<ButtonUI>(L"MadeByButton");
	m_pMadeByButton->SetWidth(50.f);
	m_pMadeByButton->SetHeight(50.f);
	m_pMadeByButton->SetColor(D2D1::ColorF::Aqua);
	m_pMadeByButton->SetRelativeLocation(-50.f, 100.f);
	m_pMadeByButton->SetYIndex(2);
	m_pMadeByButton->SetXIndex(0);
	m_pMadeByButton->SetActiveUI(false);
	//m_pMadeByButton->SetButtonEventType(eEventType::Three);
	m_pMadeByButton->AttachToComponent(m_pStartPanel);

	m_pExitButton = CreateComponent<ButtonUI>(L"ExitButton");
	m_pExitButton->SetWidth(50.f);
	m_pExitButton->SetHeight(50.f);
	m_pExitButton->SetColor(D2D1::ColorF::Blue);
	m_pExitButton->SetRelativeLocation(50.f, 100.f);
	m_pExitButton->SetYIndex(2);
	m_pExitButton->SetXIndex(2);
	m_pExitButton->SetActiveUI(false);
	//m_pExitButton->SetButtonEventType(eEventType::ChangeScene);
	m_pExitButton->AttachToComponent(m_pStartPanel);

	/*m_StartPanel = CreateComponent<PanelUI>(L"StartPanel");
	m_StartPanel->SetWidth(150.f);
	m_StartPanel->SetHeight(500.f);
	m_StartPanel->SetColor(D2D1::ColorF::White);
	m_StartPanel->SetRelativeLocation(ScreenWidth / 4.f, ScreenHeight / 2.f);
	SetRootComponent(m_StartPanel);

	m_pStartButton = CreateComponent<ButtonUI>(L"StartButton");
	m_pStartButton->SetWidth(100.f);
	m_pStartButton->SetHeight(50.f);
	m_pStartButton->SetColor(D2D1::ColorF::Red);
	m_pStartButton->SetBtnFunction(BTN_FUNC::CHANGE_SCENE);
	m_pStartButton->SetRelativeLocation(0.f, -150.f);
	m_pStartButton->SetYIndex(0);
	m_pStartButton->SetXIndex(0);
	m_pStartButton->AttachToComponent(m_StartPanel);

	m_pMethodButton = CreateComponent<ButtonUI>(L"MethodButton");
	m_pMethodButton->SetWidth(100.f);
	m_pMethodButton->SetHeight(50.f);
	m_pMethodButton->SetColor(D2D1::ColorF::Green);
	m_pMethodButton->SetBtnFunction(BTN_FUNC::PAUSE_UI);
	m_pMethodButton->SetRelativeLocation(0.f, -50.f);
	m_pMethodButton->SetYIndex(1);
	m_pMethodButton->SetXIndex(0);
	m_pMethodButton->AttachToComponent(m_StartPanel);

	m_pMadeByButton = CreateComponent<ButtonUI>(L"MadeByButton");
	m_pMadeByButton->SetWidth(100.f);
	m_pMadeByButton->SetHeight(50.f);
	m_pMadeByButton->SetColor(D2D1::ColorF::Aqua);
	m_pMadeByButton->SetBtnFunction(BTN_FUNC::DELTE_PARENT_UI);
	m_pMadeByButton->SetRelativeLocation(0.f, 50.f);
	m_pMadeByButton->SetYIndex(2);
	m_pMadeByButton->SetXIndex(0);
	m_pMadeByButton->AttachToComponent(m_StartPanel);

	m_pExitButton = CreateComponent<ButtonUI>(L"ExitButton");
	m_pExitButton->SetWidth(100.f);
	m_pExitButton->SetHeight(50.f);
	m_pExitButton->SetColor(D2D1::ColorF::Blue);
	m_pExitButton->SetBtnFunction(BTN_FUNC::DELTE_PARENT_UI);
	m_pExitButton->SetRelativeLocation(0.f, 150.f);
	m_pExitButton->SetYIndex(3);
	m_pExitButton->SetXIndex(0);
	m_pExitButton->AttachToComponent(m_StartPanel);*/

	__super::Initialize();

	return true;
}

void UIObject::Update(const float deltaTime)
{
	//m_StartPanel->Update(deltaTime);
	__super::Update(deltaTime);
}