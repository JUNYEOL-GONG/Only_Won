#include "framework.h"
#include "InstructionUIObject.h"

#include "../Engine/ButtonUI.h"
#include "../Engine/PanelUI.h"
#include "../Engine/EventManager.h"
#include "../Engine/TextureComponent.h"
#include "../Engine/InputManager.h"

bool InstructionUIObject::Initialize()
{
	EventManager::GetInstance()->RegisterListener(eEventType::InstructionTexture, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::ControllerTexture, dynamic_cast<EventListener*>(this));

	// UI 배치
	m_pInstructionWorldPanel = CreateComponent<PanelUI>(L"InstructionPanel");
	m_pInstructionWorldPanel->SetWidth(0);
	m_pInstructionWorldPanel->SetHeight(0);
	m_pInstructionWorldPanel->SetColor(D2D1::ColorF::White);
	m_pInstructionWorldPanel->SetRelativeLocation(0.f, 0.f);
	SetRootComponent(m_pInstructionWorldPanel);

	m_pInstructionTitlePanel = CreateComponent<PanelUI>(L"ShowPanel");
	m_pInstructionTitlePanel->SetWidth(0);
	m_pInstructionTitlePanel->SetHeight(0);
	m_pInstructionTitlePanel->SetColor(D2D1::ColorF::White);
	m_pInstructionTitlePanel->SetRelativeLocation(0.f, 0.f);
	m_pInstructionTitlePanel->SetActiveUI(false);
	m_pInstructionTitlePanel->AttachToComponent(m_pInstructionWorldPanel);

	m_pTitleTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pTitleTexture->SetTextureAsset(L"../Resource/titlebackgroun_blur.png", L"");
	m_pTitleTexture->SetRelativeLocation(ScreenWidth / 2.f, ScreenHeight / 2.f);
	m_pTitleTexture->AttachToComponent(m_pInstructionTitlePanel);

	m_pShowPanel = CreateComponent<PanelUI>(L"ShowPanel");
	m_pShowPanel->SetWidth(1200);
	m_pShowPanel->SetHeight(900);
	m_pShowPanel->SetColor(D2D1::ColorF::White);
	m_pShowPanel->SetRelativeLocation(680.f, 550.f);
	m_pShowPanel->SetActiveUI(false);
	m_pShowPanel->AttachToComponent(m_pInstructionWorldPanel);

	m_pShowTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pShowTexture->SetRelativeLocation(0.f, 0.f);
	m_pShowTexture->SetTextureAsset(L"../Resource/Gamerule_rule.png", L"");
	m_pShowTexture->AttachToComponent(m_pShowPanel);

	m_pOkayButton = CreateComponent<ButtonUI>(L"OkayButton");
	m_pOkayButton->SetWidth(180.f);
	m_pOkayButton->SetHeight(70.f);
	m_pOkayButton->SetColor(D2D1::ColorF::Red);
	m_pOkayButton->SetRelativeLocation(120.f, 1015.f);
	m_pOkayButton->SetActiveUI(false);
	m_pOkayButton->AttachToComponent(m_pInstructionWorldPanel);

	m_pCancelButton = CreateComponent<ButtonUI>(L"CancelButton");
	m_pCancelButton->SetWidth(180.f);
	m_pCancelButton->SetHeight(70.f);
	m_pCancelButton->SetColor(D2D1::ColorF::Red);
	m_pCancelButton->SetRelativeLocation(340.f, 1015.f);
	m_pCancelButton->SetActiveUI(false);
	m_pCancelButton->AttachToComponent(m_pInstructionWorldPanel);

	m_pInstructionButton = CreateComponent<ButtonUI>(L"InstructionButton");
	m_pInstructionButton->SetWidth(390.f);
	m_pInstructionButton->SetHeight(140.f);
	m_pInstructionButton->SetColor(D2D1::ColorF::White);
	m_pInstructionButton->SetRelativeLocation(1590.f, 565.f);
	m_pInstructionButton->SetYIndex(0);
	m_pInstructionButton->SetXIndex(0);
	m_pInstructionButton->SetButtonEventType(eEventType::InstructionTexture);
	m_pInstructionButton->AttachToComponent(m_pInstructionWorldPanel);

	m_pControllerButton = CreateComponent<ButtonUI>(L"InstructionButton");
	m_pControllerButton->SetWidth(390.f);
	m_pControllerButton->SetHeight(140.f);
	m_pControllerButton->SetColor(D2D1::ColorF::White);
	m_pControllerButton->SetRelativeLocation(1590.f, 700.f);
	m_pControllerButton->SetYIndex(1);
	m_pControllerButton->SetXIndex(0);
	m_pControllerButton->SetButtonEventType(eEventType::ControllerTexture);
	m_pControllerButton->AttachToComponent(m_pInstructionWorldPanel);

	m_pExitButton = CreateComponent<ButtonUI>(L"ExitButton");
	m_pExitButton->SetWidth(390.f);
	m_pExitButton->SetHeight(140.f);
	m_pExitButton->SetColor(D2D1::ColorF::White);
	m_pExitButton->SetRelativeLocation(1590.f, 830.f);
	m_pExitButton->SetYIndex(2);
	m_pExitButton->SetXIndex(0);
	m_pExitButton->SetButtonEventType(eEventType::ChangeWorldToMain);
	m_pExitButton->AttachToComponent(m_pInstructionWorldPanel);

	m_pSignWoodTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pSignWoodTexture->SetTextureAsset(L"../Resource/Title_Menu_Ui_.png", L"GameTitleTexture");
	m_pSignWoodTexture->SetRelativeLocation(1630.f, 725.f);
	m_pSignWoodTexture->AttachToComponent(m_pInstructionTitlePanel);

	m_pOkayTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pOkayTexture->SetTextureAsset(L"../Resource/Select.png", L"GameStartTexture");
	m_pOkayTexture->SetRelativeLocation(0.f, 0.f);
	m_pOkayTexture->AttachToComponent(m_pOkayButton);

	m_pCancelTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pCancelTexture->SetTextureAsset(L"../Resource/Cancel.png", L"GameStartTexture");
	m_pCancelTexture->SetRelativeLocation(0.f, 0.f);
	m_pCancelTexture->AttachToComponent(m_pCancelButton);

	//게임 방법 텍스쳐(비활성화)
	m_pInstructionTexture = CreateComponent<TextureComponent>(L"DeactiveTextureComponent");
	m_pInstructionTexture->SetTextureAsset(L"../Resource/Title_Menu_howtoplay_withrope_unactived.png", L"GameStartTexture");
	m_pInstructionTexture->SetRelativeLocation(0.f, 0.f);
	m_pInstructionTexture->SetShowTexture(true);
	m_pInstructionTexture->AttachToComponent(m_pInstructionButton);

	//게임 방법 텍스쳐(활성화)
	m_pActiveInstructionTexture = CreateComponent<TextureComponent>(L"ActiveTextureComponent");
	m_pActiveInstructionTexture->SetTextureAsset(L"../Resource/Title_Menu_howtoplay_withrope.png", L"GameStartTexture");
	m_pActiveInstructionTexture->SetRelativeLocation(0.f, 0.f);
	m_pActiveInstructionTexture->SetShowTexture(false);
	m_pActiveInstructionTexture->AttachToComponent(m_pInstructionButton);

	//컨트롤러 텍스쳐(비활성화)
	m_pControllerTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pControllerTexture->SetTextureAsset(L"../Resource/Menu_explanation_controller_withrope_unactived.png", L"GameStartTexture");
	m_pControllerTexture->SetRelativeLocation(0.f, 0.f);
	m_pControllerTexture->AttachToComponent(m_pControllerButton);

	//컨트롤러 텍스쳐(활성화)
	m_pActiveControllerTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pActiveControllerTexture->SetTextureAsset(L"../Resource/Menu_explanation_controller_withrope.png", L"GameStartTexture");
	m_pActiveControllerTexture->SetRelativeLocation(0.f, 0.f);
	m_pActiveControllerTexture->SetShowTexture(false);
	m_pActiveControllerTexture->AttachToComponent(m_pControllerButton);

	// 이전 텍스쳐(비활성화)
	m_pExitTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pExitTexture->SetTextureAsset(L"../Resource/Menu_back_withrope_unactived.png", L"GameStartTexture");
	m_pExitTexture->SetRelativeLocation(0.f, 0.f);
	m_pExitTexture->AttachToComponent(m_pExitButton);

	// 이전 텍스쳐(활성화)
	m_pActiveExitTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pActiveExitTexture->SetTextureAsset(L"../Resource/Menu_back_withrope.png", L"GameStartTexture");
	m_pActiveExitTexture->SetRelativeLocation(0.f, 0.f);
	m_pActiveExitTexture->SetShowTexture(false);
	m_pActiveExitTexture->AttachToComponent(m_pExitButton);

	__super::Initialize();

	return true;
}

void InstructionUIObject::Update(const float deltaTime)
{
	if (InputManager::GetInstance()->IsPadButtonPush(0, GamePadButtonCode::B))
		EventManager::GetInstance()->SendEvent(eEventType::ChangeWorldToMain);
	__super::Update(deltaTime);
}

void InstructionUIObject::HandleEvent(Event* event)
{
	if(event->eventID == eEventType::InstructionTexture)
		m_pShowTexture->SetTextureAsset(L"../Resource/Gamerule_rule.png", L"");
	if (event->eventID == eEventType::ControllerTexture)
		m_pShowTexture->SetTextureAsset(L"../Resource/Gamerule_controller.png", L"");
}
