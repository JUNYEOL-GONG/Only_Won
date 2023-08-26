#include "framework.h"
#include "GameSettingUIObject.h"

#include "../Engine/ButtonUI.h"
#include "../Engine/PanelUI.h"
#include "../Engine/EventManager.h"
#include "../Engine/TextureComponent.h"
#include "../Engine/InputManager.h"

bool GameSettingUIObject::Initialize()
{
	EventManager::GetInstance()->RegisterListener(eEventType::SetMap1, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::SetMap2, dynamic_cast<EventListener*>(this));

	// UI 배치
	m_pRootPanel = CreateComponent<PanelUI>(L"GameSettingPanel");
	m_pRootPanel->SetWidth(0);
	m_pRootPanel->SetHeight(0);
	m_pRootPanel->SetColor(D2D1::ColorF::White);
	m_pRootPanel->SetRelativeLocation(0.f, 0.f);
	SetRootComponent(m_pRootPanel);

	m_pGameSettingPanel = CreateComponent<PanelUI>(L"GameSettingPanel");
	m_pGameSettingPanel->SetWidth(0);
	m_pGameSettingPanel->SetHeight(0);
	m_pGameSettingPanel->SetColor(D2D1::ColorF::White);
	m_pGameSettingPanel->SetRelativeLocation(0.f, 0.f);
	m_pGameSettingPanel->SetActiveUI(false);
	m_pGameSettingPanel->AttachToComponent(m_pRootPanel);

	m_pBackgroundTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pBackgroundTexture->SetTextureAsset(L"../Resource/titlebackgroun_blur.png", L"GameStartTexture");
	m_pBackgroundTexture->SetRelativeLocation(ScreenWidth / 2.f, ScreenHeight / 2.f);
	m_pBackgroundTexture->AttachToComponent(m_pGameSettingPanel);

	m_pMapImage = CreateComponent<PanelUI>(L"MapImagePanel");
	m_pMapImage->SetWidth(940);
	m_pMapImage->SetHeight(520);
	m_pMapImage->SetColor(D2D1::ColorF::Red);
	m_pMapImage->SetRelativeLocation(550.f, 520.f);
	m_pMapImage->SetActiveUI(false);
	m_pMapImage->AttachToComponent(m_pRootPanel);

	m_pUsingItemTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pUsingItemTexture->SetTextureAsset(L"../Resource/사용아이템목록.png", L"");
	m_pUsingItemTexture->SetRelativeLocation(1470.f, 655.f);
	m_pUsingItemTexture->AttachToComponent(m_pGameSettingPanel);

	m_pLogoTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pLogoTexture->SetTextureAsset(L"../Resource/Title_Onlywon.png", L"");
	m_pLogoTexture->SetRelativeLocation(1610.f, 200.f);
	m_pLogoTexture->AttachToComponent(m_pGameSettingPanel);

	m_pBigMapTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pBigMapTexture->SetTextureAsset(L"../Resource/Select_Map1_Large.png", L"");
	m_pBigMapTexture->SetRelativeLocation(0.f, 0.f);
	m_pBigMapTexture->AttachToComponent(m_pMapImage);

	m_pOkayButton = CreateComponent<ButtonUI>(L"SelectButton");
	m_pOkayButton->SetWidth(180.f);
	m_pOkayButton->SetHeight(70.f);
	m_pOkayButton->SetColor(D2D1::ColorF::Red);
	m_pOkayButton->SetRelativeLocation(120.f, 1015.f);
	m_pOkayButton->SetActiveUI(false);
	m_pOkayButton->AttachToComponent(m_pRootPanel);

	m_pCancelButton = CreateComponent<ButtonUI>(L"CancelButton");
	m_pCancelButton->SetWidth(180.f);
	m_pCancelButton->SetHeight(70.f);
	m_pCancelButton->SetColor(D2D1::ColorF::Red);
	m_pCancelButton->SetRelativeLocation(340.f, 1015.f);
	m_pCancelButton->SetActiveUI(false);
	m_pCancelButton->AttachToComponent(m_pRootPanel);

	m_pSelectMap1 = CreateComponent<ButtonUI>(L"SelectMap1");
	m_pSelectMap1->SetWidth(300.f);
	m_pSelectMap1->SetHeight(160.f);
	m_pSelectMap1->SetColor(D2D1::ColorF::White);
	m_pSelectMap1->SetRelativeLocation(230.f, 890.f);
	m_pSelectMap1->SetYIndex(0);
	m_pSelectMap1->SetXIndex(0);
	m_pSelectMap1->SetButtonEventType(eEventType::SetMap1);
	m_pSelectMap1->AttachToComponent(m_pRootPanel);

	m_pSmallMap1Texture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pSmallMap1Texture->SetTextureAsset(L"../Resource/Select_Map1_Small.png", L"");
	m_pSmallMap1Texture->SetRelativeLocation(0.f, 0.f);
	m_pSmallMap1Texture->AttachToComponent(m_pSelectMap1);

	m_pSelectMap2 = CreateComponent<ButtonUI>(L"SelectMap2");
	m_pSelectMap2->SetWidth(300.f);
	m_pSelectMap2->SetHeight(160.f);
	m_pSelectMap2->SetColor(D2D1::ColorF::White);
	m_pSelectMap2->SetRelativeLocation(550.f, 890.f);
	m_pSelectMap2->SetYIndex(0);
	m_pSelectMap2->SetXIndex(1);
	m_pSelectMap2->SetButtonEventType(eEventType::SetMap2);
	m_pSelectMap2->AttachToComponent(m_pRootPanel);

	m_pSmallMap2Texture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pSmallMap2Texture->SetTextureAsset(L"../Resource/Select_Map2_Small.png", L"");
	m_pSmallMap2Texture->SetRelativeLocation(0.f, 0.f);
	m_pSmallMap2Texture->AttachToComponent(m_pSelectMap2);

	/*m_pSelectMap3 = CreateComponent<ButtonUI>(L"SelectMap3");
	m_pSelectMap3->SetWidth(300.f);
	m_pSelectMap3->SetHeight(160.f);
	m_pSelectMap3->SetColor(D2D1::ColorF::White);
	m_pSelectMap3->SetRelativeLocation(870.f, 890.f);
	m_pSelectMap3->SetYIndex(0);
	m_pSelectMap3->SetXIndex(2);
	m_pSelectMap3->AttachToComponent(m_pRootPanel);*/

	m_pStartButton = CreateComponent<ButtonUI>(L"StartButton");
	m_pStartButton->SetWidth(780.f);
	m_pStartButton->SetHeight(110.f);
	m_pStartButton->SetColor(D2D1::ColorF::White);
	m_pStartButton->SetRelativeLocation(1470.f, 925.f);
	m_pStartButton->SetYIndex(0);
	m_pStartButton->SetXIndex(2);
	m_pStartButton->SetButtonEventType(eEventType::ChangeWorldToInGame);
	m_pStartButton->AttachToComponent(m_pRootPanel);

	m_pStartTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pStartTexture->SetTextureAsset(L"../Resource/gamestart_unactived.png", L"GameStartTexture");
	m_pStartTexture->SetRelativeLocation(0.f, 0.f);
	m_pStartTexture->AttachToComponent(m_pStartButton);

	m_pActiveStartTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pActiveStartTexture->SetTextureAsset(L"../Resource/gamestart.png", L"GameStartTexture");
	m_pActiveStartTexture->SetShowTexture(false);
	m_pActiveStartTexture->SetRelativeLocation(0.f, 0.f);
	m_pActiveStartTexture->AttachToComponent(m_pStartButton);

	__super::Initialize();

	return true;
}

void GameSettingUIObject::Update(const float deltaTime)
{
	if (InputManager::GetInstance()->IsPadButtonPush(0, GamePadButtonCode::B))
		EventManager::GetInstance()->SendEvent(eEventType::ChangeWorldToMain);

	__super::Update(deltaTime);
}

void GameSettingUIObject::HandleEvent(Event* event)
{
	if(event->eventID == eEventType::SetMap1)
	{
		m_pStartButton->SetButtonEventType(eEventType::ChangeWorldToInGame);
		m_pBigMapTexture->SetShowTexture(true);
		m_pBigMapTexture->SetTextureAsset(L"../Resource/Select_Map1_Large.png", L"");
	}
	if (event->eventID == eEventType::SetMap2)
	{
		m_pStartButton->SetButtonEventType(eEventType::ChangeWorldToInGame2);
		m_pBigMapTexture->SetShowTexture(true);
		m_pBigMapTexture->SetTextureAsset(L"../Resource/Select_Map2_Large.png", L"");
	}
}
