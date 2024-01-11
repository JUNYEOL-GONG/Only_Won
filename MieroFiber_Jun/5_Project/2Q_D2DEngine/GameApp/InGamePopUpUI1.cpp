#include "framework.h"
#include "InGamePopUpUI1.h"

#include "../Engine/ButtonUI.h"
#include "../Engine/PopUpUI.h"
#include "../Engine/EventManager.h"
#include "../Engine/PanelUI.h"
#include "../Engine/World.h"

bool InGamePopUpUI1::Initialize()
{
	EventManager::GetInstance()->RegisterListener(eEventType::CheckQuitGame2, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::Continue2, dynamic_cast<EventListener*>(this));

	m_pRootPanel = CreateComponent<PanelUI>(L"PopUpUI");
	m_pRootPanel->SetWidth(0);
	m_pRootPanel->SetHeight(0);
	m_pRootPanel->SetColor(D2D1::ColorF::White);
	m_pRootPanel->SetRelativeLocation(0.f, 0.f);
	SetRootComponent(m_pRootPanel);

	m_pPopUpPanel = CreateComponent<PanelUI>(L"PopUpUI");
	m_pPopUpPanel->SetWidth(0);
	m_pPopUpPanel->SetHeight(0);
	m_pPopUpPanel->SetColor(D2D1::ColorF::White);
	m_pPopUpPanel->SetRelativeLocation(0.f, 0.f);
	m_pPopUpPanel->SetActiveUI(false);
	m_pPopUpPanel->AttachToComponent(m_pRootPanel);

	m_pPausePanel = CreateComponent<PanelUI>(L"PopUpUI");
	m_pPausePanel->SetWidth(800.f);
	m_pPausePanel->SetHeight(700.f);
	m_pPausePanel->SetColor(D2D1::ColorF::White);
	m_pPausePanel->SetRelativeLocation(960.f, 540.f);
	m_pPausePanel->SetActiveUI(false);
	m_pPausePanel->AttachToComponent(m_pRootPanel);

	// 배경
	m_pBackgroundTexture = CreateComponent<TextureComponent>(L"BackgroundPanel");
	m_pBackgroundTexture->SetTextureAsset(L"../Resource/pause_frame.png", L"Clear");
	m_pBackgroundTexture->SetRelativeLocation(0.f,0.f);
	m_pBackgroundTexture->AttachToComponent(m_pPausePanel);

	// 일시정지
	m_pPauseTexture = CreateComponent<TextureComponent>(L"PauseTexture");
	m_pPauseTexture->SetTextureAsset(L"../Resource/pause_01.png", L"Clear");
	m_pPauseTexture->SetRelativeLocation(962.5f, 431.5f);
	m_pPauseTexture->AttachToComponent(m_pPopUpPanel);

	// 첫번째
	// 이어하기
	m_pContinueButton = CreateComponent<ButtonUI>(L"OkayButton");
	m_pContinueButton->SetWidth(300.f);
	m_pContinueButton->SetHeight(110.f);
	m_pContinueButton->SetColor(D2D1::ColorF::Red);
	m_pContinueButton->SetRelativeLocation(960.f, 585.f);
	m_pContinueButton->SetYIndex(0);
	m_pContinueButton->SetXIndex(0);
	m_pContinueButton->SetButtonEventType(eEventType::Continue);
	m_pContinueButton->AttachToComponent(m_pRootPanel);

	m_pContinueTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pContinueTexture->SetRelativeLocation(0.f, 0.f);
	m_pContinueTexture->SetTextureAsset(L"../Resource/pause_button_unactived_continue.png", L"GameStartTexture");
	m_pContinueTexture->AttachToComponent(m_pContinueButton);

	m_pActiveContinueTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pActiveContinueTexture->SetRelativeLocation(0.f, 0.f);
	m_pActiveContinueTexture->SetTextureAsset(L"../Resource/pause_button_continue.png", L"GameStartTexture");
	m_pActiveContinueTexture->SetShowTexture(false);
	m_pActiveContinueTexture->AttachToComponent(m_pContinueButton);

	// 메인메뉴
	m_pMainMenuButton = CreateComponent<ButtonUI>(L"NoButton");
	m_pMainMenuButton->SetWidth(300.f);
	m_pMainMenuButton->SetHeight(110.f);
	m_pMainMenuButton->SetColor(D2D1::ColorF::Green);
	m_pMainMenuButton->SetRelativeLocation(960.f, 725.f);
	m_pMainMenuButton->SetYIndex(1);
	m_pMainMenuButton->SetXIndex(0);
	m_pMainMenuButton->SetButtonEventType(eEventType::CheckQuitGame2);
	m_pMainMenuButton->AttachToComponent(m_pRootPanel);
	
	m_pMainMenuTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pMainMenuTexture->SetRelativeLocation(0.f, 0.f);
	m_pMainMenuTexture->SetTextureAsset(L"../Resource/pause_button_unactived_mainmenu.png", L"GameStartTexture");
	m_pMainMenuTexture->AttachToComponent(m_pMainMenuButton);

	m_pActiveMainMenuTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pActiveMainMenuTexture->SetRelativeLocation(0.f, 0.f);
	m_pActiveMainMenuTexture->SetTextureAsset(L"../Resource/pause_button_mainmenu.png", L"GameStartTexture");
	m_pActiveMainMenuTexture->SetShowTexture(false);
	m_pActiveMainMenuTexture->AttachToComponent(m_pMainMenuButton);

	// 두번째
	// 메인 메뉴로 돌아가시겠습니까?
	m_pGoToMainMenu = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pGoToMainMenu->SetRelativeLocation(952.f, 520.f);
	m_pGoToMainMenu->SetTextureAsset(L"../Resource/turnback_check_text.png", L"GameStartTexture");
	m_pGoToMainMenu->SetShowTexture(false);
	m_pGoToMainMenu->AttachToComponent(m_pPopUpPanel);

	// 예
	m_pYesButton = CreateComponent<ButtonUI>(L"YesButton");
	m_pYesButton->SetWidth(300.f);
	m_pYesButton->SetHeight(110.f);
	m_pYesButton->SetColor(D2D1::ColorF::Red);
	m_pYesButton->SetRelativeLocation(810.f, 668.f);
	m_pYesButton->SetYIndex(0);
	m_pYesButton->SetXIndex(0);
	m_pYesButton->SetActiveUI(false);
	m_pYesButton->SetButtonEventType(eEventType::ChangeWorldToMain);
	m_pYesButton->AttachToComponent(m_pRootPanel);

	m_pYesTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pYesTexture->SetRelativeLocation(0.f, 0.f);
	m_pYesTexture->SetTextureAsset(L"../Resource/turnback_check_unactived_yes.png", L"GameStartTexture");
	m_pYesTexture->SetShowTexture(false);
	m_pYesTexture->AttachToComponent(m_pYesButton);

	m_pActiveYesTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pActiveYesTexture->SetRelativeLocation(0.f, 0.f);
	m_pActiveYesTexture->SetTextureAsset(L"../Resource/turnback_check_yes.png", L"GameStartTexture");
	m_pActiveYesTexture->SetShowTexture(false);
	m_pActiveYesTexture->AttachToComponent(m_pYesButton);

	// 아니요
	m_pNoButton = CreateComponent<ButtonUI>(L"NoButton");
	m_pNoButton->SetWidth(300.f);
	m_pNoButton->SetHeight(110.f);
	m_pNoButton->SetColor(D2D1::ColorF::Red);
	m_pNoButton->SetRelativeLocation(1100.f, 668.f);
	m_pNoButton->SetYIndex(0);
	m_pNoButton->SetXIndex(1);
	m_pNoButton->SetActiveUI(false);
	m_pNoButton->SetButtonEventType(eEventType::Continue2);
	m_pNoButton->AttachToComponent(m_pRootPanel);

	m_pNoTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pNoTexture->SetRelativeLocation(0.f, 0.f);
	m_pNoTexture->SetTextureAsset(L"../Resource/turnback_check_unactived_no.png", L"GameStartTexture");
	m_pNoTexture->SetShowTexture(false);
	m_pNoTexture->AttachToComponent(m_pNoButton);

	m_pActiveNoTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pActiveNoTexture->SetRelativeLocation(0.f, 0.f);
	m_pActiveNoTexture->SetTextureAsset(L"../Resource/turnback_check_no.png", L"GameStartTexture");
	m_pActiveNoTexture->SetShowTexture(false);
	m_pActiveNoTexture->AttachToComponent(m_pNoButton);

	__super::Initialize();

	return true;
}

void InGamePopUpUI1::Update(const float deltaTime)
{
	__super::Update(deltaTime);
}

void InGamePopUpUI1::HandleEvent(Event* event)
{
	if(event->eventID == eEventType::CheckQuitGame2)
	{
		m_pContinueButton->SetActiveUI(false);
		m_pContinueTexture->SetShowTexture(false);
		m_pActiveContinueTexture->SetShowTexture(false);
		m_pMainMenuButton->SetActiveUI(false);
		m_pMainMenuTexture->SetShowTexture(false);
		m_pActiveMainMenuTexture->SetShowTexture(false);

		m_pGoToMainMenu->SetShowTexture(true);
		m_pYesButton->SetActiveUI(true);
		m_pYesTexture->SetShowTexture(true);
		m_pActiveYesTexture->SetShowTexture(false);
		m_pNoButton->SetActiveUI(true);
		m_pNoTexture->SetShowTexture(true);
		m_pActiveNoTexture->SetShowTexture(false);
	}
	if (event->eventID == eEventType::Continue2)
	{
		m_pContinueButton->SetActiveUI(true);
		m_pContinueTexture->SetShowTexture(true);
		m_pActiveContinueTexture->SetShowTexture(false);
		m_pMainMenuButton->SetActiveUI(true);
		m_pMainMenuTexture->SetShowTexture(true);
		m_pActiveMainMenuTexture->SetShowTexture(false);

		m_pGoToMainMenu->SetShowTexture(false);
		m_pYesButton->SetActiveUI(false);
		m_pYesTexture->SetShowTexture(false);
		m_pActiveYesTexture->SetShowTexture(false);
		m_pNoButton->SetActiveUI(false);
		m_pNoTexture->SetShowTexture(false);
		m_pActiveNoTexture->SetShowTexture(false);

		this->GetOwnerWorld()->SetWorldPauseState(false);
		this->SetActiveObj(false);
		this->SetPopUpState(false);
	}
}
