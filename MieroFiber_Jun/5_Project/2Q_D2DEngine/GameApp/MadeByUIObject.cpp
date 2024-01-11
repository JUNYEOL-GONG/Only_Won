#include "framework.h"
#include "MadeByUIObject.h"

#include "../Engine/ButtonUI.h"
#include "../Engine/PanelUI.h"
#include "../Engine/EventManager.h"
#include "../Engine/TextureComponent.h"
#include "../Engine/InputManager.h"

bool MadeByUIObject::Initialize()
{
	// UI ¹èÄ¡
	m_pRootPanel = CreateComponent<PanelUI>(L"StartPanel");
	m_pRootPanel->SetWidth(0);
	m_pRootPanel->SetHeight(0);
	m_pRootPanel->SetColor(D2D1::ColorF::White);
	m_pRootPanel->SetRelativeLocation(0.f, 0.f);
	SetRootComponent(m_pRootPanel);

	m_pMadeByPanel = CreateComponent<PanelUI>(L"StartPanel");
	m_pMadeByPanel->SetWidth(0);
	m_pMadeByPanel->SetHeight(0);
	m_pMadeByPanel->SetColor(D2D1::ColorF::White);
	m_pMadeByPanel->SetRelativeLocation(0.f, 0.f);
	m_pMadeByPanel->AttachToComponent(m_pRootPanel);

	m_pBackgroundTexture = CreateComponent<TextureComponent>(L"BackgroundTexture");
	m_pBackgroundTexture->SetRelativeLocation(ScreenWidth / 2.f, ScreenHeight / 2.f);
	m_pBackgroundTexture->SetTextureAsset(L"../Resource/titlebackgroun_blur.png", L"");
	m_pBackgroundTexture->AttachToComponent(m_pMadeByPanel);

	m_pShowPanel = CreateComponent<PanelUI>(L"ShowPanel");
	m_pShowPanel->SetWidth(0);
	m_pShowPanel->SetHeight(0);
	m_pShowPanel->SetColor(D2D1::ColorF::White);
	m_pShowPanel->SetRelativeLocation(0.f, 0.f);
	m_pShowPanel->SetActiveUI(false);
	m_pShowPanel->AttachToComponent(m_pRootPanel);

	m_pShowTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pShowTexture->SetTextureAsset(L"../Resource/credit_ui.png", L"GameStartTexture");
	m_pShowTexture->SetRelativeLocation(960.f, 545.f);
	m_pShowTexture->AttachToComponent(m_pShowPanel);

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

	m_pPrevButton = CreateComponent<ButtonUI>(L"PrevButton");
	m_pPrevButton->SetWidth(390.f);
	m_pPrevButton->SetHeight(110.f);
	m_pPrevButton->SetColor(D2D1::ColorF::Red);
	m_pPrevButton->SetRelativeLocation(960.f, 995.f);
	m_pPrevButton->SetYIndex(0);
	m_pPrevButton->SetXIndex(0);
	m_pPrevButton->SetButtonEventType(eEventType::ChangeWorldToMain);
	m_pPrevButton->AttachToComponent(m_pRootPanel);

	m_pPrevTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPrevTexture->SetTextureAsset(L"../Resource/Menu_back.png", L"GameStartTexture");
	m_pPrevTexture->SetRelativeLocation(0.f, 0.f);
	m_pPrevTexture->AttachToComponent(m_pPrevButton);

	m_pOkayTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pOkayTexture->SetTextureAsset(L"../Resource/Select.png", L"GameStartTexture");
	m_pOkayTexture->SetRelativeLocation(0.f, 0.f);
	m_pOkayTexture->AttachToComponent(m_pOkayButton);

	m_pCancelTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pCancelTexture->SetTextureAsset(L"../Resource/Cancel.png", L"GameStartTexture");
	m_pCancelTexture->SetRelativeLocation(0.f, 0.f);
	m_pCancelTexture->AttachToComponent(m_pCancelButton);

	__super::Initialize();

	return true;
}

void MadeByUIObject::Update(const float deltaTime)
{
	if (InputManager::GetInstance()->IsPadButtonPush(0, GamePadButtonCode::B))
		EventManager::GetInstance()->SendEvent(eEventType::ChangeWorldToMain);
	__super::Update(deltaTime);
}