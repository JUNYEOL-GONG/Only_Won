#include "framework.h"
#include "MainUIObject.h"

#include "../Engine/ButtonUI.h"
#include "../Engine/PanelUI.h"
#include "../Engine/EventManager.h"
#include "../Engine/InputManager.h"
#include "../Engine/TextureComponent.h"

bool MainUIObject::Initialize()
{
	// UI 배치
	m_pMainWorldPanel = CreateComponent<PanelUI>(L"StartPanel");
	m_pMainWorldPanel->SetWidth(0);
	m_pMainWorldPanel->SetHeight(0);
	m_pMainWorldPanel->SetColor(D2D1::ColorF::White);
	m_pMainWorldPanel->SetRelativeLocation(0.f, 0.f);
	SetRootComponent(m_pMainWorldPanel);

	m_pTitlePanel = CreateComponent<PanelUI>(L"TitlePanel");
	m_pTitlePanel->SetWidth(0);
	m_pTitlePanel->SetHeight(0);
	m_pTitlePanel->SetColor(D2D1::ColorF::White);
	m_pTitlePanel->SetRelativeLocation(0.f, 0.f);
	m_pTitlePanel->SetActiveUI(false);
	m_pTitlePanel->AttachToComponent(m_pMainWorldPanel);

	m_pOkayButton = CreateComponent<ButtonUI>(L"SelectButton");
	m_pOkayButton->SetWidth(180.f);
	m_pOkayButton->SetHeight(70.f);
	m_pOkayButton->SetColor(D2D1::ColorF::Red);
	m_pOkayButton->SetRelativeLocation(120.f, 1015.f);
	m_pOkayButton->SetYIndex(3);
	m_pOkayButton->SetXIndex(0);
	m_pOkayButton->SetActiveUI(false);
	m_pOkayButton->AttachToComponent(m_pMainWorldPanel);

	m_pCancelButton = CreateComponent<ButtonUI>(L"CancelButton");
	m_pCancelButton->SetWidth(180.f);
	m_pCancelButton->SetHeight(70.f);
	m_pCancelButton->SetColor(D2D1::ColorF::Red);
	m_pCancelButton->SetRelativeLocation(340.f, 1015.f);
	m_pCancelButton->SetYIndex(3);
	m_pCancelButton->SetXIndex(1);
	m_pCancelButton->SetActiveUI(false);
	m_pCancelButton->AttachToComponent(m_pMainWorldPanel);

	m_pStartButton = CreateComponent<ButtonUI>(L"StartButton");
	m_pStartButton->SetWidth(390.f);
	m_pStartButton->SetHeight(140.f);
	m_pStartButton->SetColor(D2D1::ColorF::White);
	m_pStartButton->SetRelativeLocation(1590.f, 565.f);
	m_pStartButton->SetYIndex(0);
	m_pStartButton->SetXIndex(2);
	m_pStartButton->SetButtonEventType(eEventType::ChangeWorldToGameSetting);
	m_pStartButton->AttachToComponent(m_pMainWorldPanel);

	m_pInstructionButton = CreateComponent<ButtonUI>(L"InstructionButton");
	m_pInstructionButton->SetWidth(390.f);
	m_pInstructionButton->SetHeight(140.f);
	m_pInstructionButton->SetColor(D2D1::ColorF::White);
	m_pInstructionButton->SetRelativeLocation(1590.f, 700.f);
	m_pInstructionButton->SetYIndex(1);
	m_pInstructionButton->SetXIndex(2);
	m_pInstructionButton->SetButtonEventType(eEventType::ChangeWorldToInstruction);
	m_pInstructionButton->AttachToComponent(m_pMainWorldPanel);

	m_pMadeByButton = CreateComponent<ButtonUI>(L"MadeByButton");
	m_pMadeByButton->SetWidth(390.f);
	m_pMadeByButton->SetHeight(140.f);
	m_pMadeByButton->SetColor(D2D1::ColorF::White);
	m_pMadeByButton->SetRelativeLocation(1590.f, 830.f);
	m_pMadeByButton->SetYIndex(2);
	m_pMadeByButton->SetXIndex(2);
	m_pMadeByButton->SetButtonEventType(eEventType::ChangeWorldToMadeBy);
	m_pMadeByButton->AttachToComponent(m_pMainWorldPanel);

	m_pExitButton = CreateComponent<ButtonUI>(L"ExitButton");
	m_pExitButton->SetWidth(390.f);
	m_pExitButton->SetHeight(140.f);
	m_pExitButton->SetColor(D2D1::ColorF::White);
	m_pExitButton->SetRelativeLocation(1590.f, 955.f);
	m_pExitButton->SetYIndex(3);
	m_pExitButton->SetXIndex(2);
	m_pExitButton->SetButtonEventType(eEventType::GameEnd);
	m_pExitButton->AttachToComponent(m_pMainWorldPanel);

	// 텍스처 입히기
	m_pTitleTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pTitleTexture->SetTextureAsset(L"../Resource/Title.png", L"GameTitleTexture");
	m_pTitleTexture->SetRelativeLocation(ScreenWidth / 2.f, ScreenHeight / 2.f);
	m_pTitleTexture->AttachToComponent(m_pTitlePanel);

	m_pSignWoodTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pSignWoodTexture->SetTextureAsset(L"../Resource/Title_Menu_Ui_.png", L"GameTitleTexture");
	m_pSignWoodTexture->SetRelativeLocation(1630.f, 725.f);
	m_pSignWoodTexture->AttachToComponent(m_pTitlePanel);

	m_pOkayTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pOkayTexture->SetTextureAsset(L"../Resource/Select.png", L"GameStartTexture");
	m_pOkayTexture->SetRelativeLocation(0.f, 0.f);
	m_pOkayTexture->AttachToComponent(m_pOkayButton);

	m_pCancelTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pCancelTexture->SetTextureAsset(L"../Resource/Cancel.png", L"GameStartTexture");
	m_pCancelTexture->SetRelativeLocation(0.f, 0.f);
	m_pCancelTexture->AttachToComponent(m_pCancelButton);

	//게임 시작 텍스쳐(비활성화)
	m_pGameStartTexture = CreateComponent<TextureComponent>(L"DeactiveTextureComponent");
	m_pGameStartTexture->SetTextureAsset(L"../Resource/Title_Menu_gamestart_unactived.png", L"GameStartTexture");
	m_pGameStartTexture->SetRelativeLocation(0.f, 0.f);
	m_pGameStartTexture->SetShowTexture(true);
	m_pGameStartTexture->AttachToComponent(m_pStartButton);

	//게임 시작 텍스쳐(활성화)
	m_pActiveGameStartTexture = CreateComponent<TextureComponent>(L"ActiveTextureComponent");
	m_pActiveGameStartTexture->SetTextureAsset(L"../Resource/Title_Menu_gamestart.png", L"GameStartTexture");
	m_pActiveGameStartTexture->SetRelativeLocation(0.f, 0.f);
	m_pActiveGameStartTexture->SetShowTexture(false);
	m_pActiveGameStartTexture->AttachToComponent(m_pStartButton);

	//게임 방법 텍스쳐(비활성화)
	m_pInstructionTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pInstructionTexture->SetTextureAsset(L"../Resource/Title_Menu_howtoplay_unactived.png", L"GameStartTexture");
	m_pInstructionTexture->SetRelativeLocation(0.f, 0.f);
	m_pInstructionTexture->AttachToComponent(m_pInstructionButton);

	//게임 방법 텍스쳐(활성화)
	m_pActiveInstructionTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pActiveInstructionTexture->SetTextureAsset(L"../Resource/Title_Menu_howtoplay.png", L"GameStartTexture");
	m_pActiveInstructionTexture->SetRelativeLocation(0.f, 0.f);
	m_pActiveInstructionTexture->SetShowTexture(false);
	m_pActiveInstructionTexture->AttachToComponent(m_pInstructionButton);

	// 제작 텍스쳐(비활성화)
	m_pMadeByTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pMadeByTexture->SetTextureAsset(L"../Resource/Title_Menu_credit_unactived.png", L"GameStartTexture");
	m_pMadeByTexture->SetRelativeLocation(0.f, 0.f);
	m_pMadeByTexture->AttachToComponent(m_pMadeByButton);

	// 제작 텍스쳐(활성화)
	m_pActiveMadeByTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pActiveMadeByTexture->SetTextureAsset(L"../Resource/Title_Menu_credit.png", L"GameStartTexture");
	m_pActiveMadeByTexture->SetRelativeLocation(0.f, 0.f);
	m_pActiveMadeByTexture->SetShowTexture(false);
	m_pActiveMadeByTexture->AttachToComponent(m_pMadeByButton);

	// 끝내기 텍스쳐(비활성화)
	m_pExitTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pExitTexture->SetTextureAsset(L"../Resource/Title_Menu_exit_unactived.png", L"GameStartTexture");
	m_pExitTexture->SetRelativeLocation(0.f, 0.f);
	m_pExitTexture->AttachToComponent(m_pExitButton);

	// 끝내기 텍스쳐(활성화)
	m_pActiveExitTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pActiveExitTexture->SetTextureAsset(L"../Resource/Title_Menu_exit.png", L"GameStartTexture");
	m_pActiveExitTexture->SetRelativeLocation(0.f, 0.f);
	m_pActiveExitTexture->SetShowTexture(false);
	m_pActiveExitTexture->AttachToComponent(m_pExitButton);

	__super::Initialize();

	return true;
}

void MainUIObject::Update(const float deltaTime)
{
	if(InputManager::GetInstance()->IsPadButtonPush(0, GamePadButtonCode::B))
		PostQuitMessage(0);
	__super::Update(deltaTime);
}