#include "framework.h"
#include "InGameResultPopUp.h"

#include "../Engine/AnimationComponent.h"
#include "../Engine/ButtonUI.h"
#include "../Engine/PanelUI.h"
#include "../Engine/TextureComponent.h"
#include "../Engine/EventManager.h"
#include "../Engine/SoundManager.h"
#include "../Engine/InputManager.h"

bool InGameResultPopUp::Initialize()
{
	m_musicPlay1 = false;
	m_musicPlay2 = false;

	m_pRootPanel = CreateComponent<PanelUI>(L"ResultRootPanel");
	m_pRootPanel->SetWidth(0.f);
	m_pRootPanel->SetHeight(0.f);
	m_pRootPanel->SetColor(D2D1::ColorF::White);
	m_pRootPanel->SetRelativeLocation(0.f, 0.f);
	SetRootComponent(m_pRootPanel);

	m_pMainResultPanel = CreateComponent<PanelUI>(L"ResultRootPanel");
	m_pMainResultPanel->SetWidth(0.f);
	m_pMainResultPanel->SetHeight(0.f);
	m_pMainResultPanel->SetColor(D2D1::ColorF::White);
	m_pMainResultPanel->SetRelativeLocation(0.f, 0.f);
	m_pMainResultPanel->AttachToComponent(m_pRootPanel);

	m_pResultPanel = CreateComponent<PanelUI>(L"ResultPopUpUI");
	m_pResultPanel->SetWidth(800.f);
	m_pResultPanel->SetHeight(1000.f);
	m_pResultPanel->SetColor(D2D1::ColorF::White);
	m_pResultPanel->SetRelativeLocation(ScreenWidth / 2.f, ScreenHeight / 2.f);
	m_pResultPanel->SetActiveUI(false);
	m_pResultPanel->AttachToComponent(m_pRootPanel);

	// Ending Before(투명도 있는 까만 화면)
	m_pEndingBeforeTexture = CreateComponent<TextureComponent>(L"EndingBefore");
	m_pEndingBeforeTexture->SetRelativeLocation(0.f, 0.f);
	m_pEndingBeforeTexture->SetTextureAsset(L"../Resource/EndingBefore.png", L"EndingBefore");
	m_pEndingBeforeTexture->AttachToComponent(m_pResultPanel);

	// Ending After(까만 화면 + 노란색 스포트라이트)
	m_pEndingAfterTexture = CreateComponent<TextureComponent>(L"EndingAfter");
	m_pEndingAfterTexture->SetRelativeLocation(0.f, 0.f);
	m_pEndingAfterTexture->SetTextureAsset(L"../Resource/EndingAfter.png", L"EndingAfter");
	m_pEndingAfterTexture->AttachToComponent(m_pResultPanel);
	m_pEndingAfterTexture->SetVisible(false);

	m_pWinnerIdle = CreateComponent<AnimationComponent>(L"IdleAnimation");
	m_pWinnerIdle->SetRelativeLocation(m_winnerPoint.x, m_winnerPoint.y);
	SetWinnerAnimtionAsset();
	m_pWinnerIdle->AttachToComponent(m_pResultPanel);
	m_pWinnerIdle->SetVisible(false);

	m_pExclamation = CreateComponent<TextureComponent>(L"ExclamationTexture");
	m_pExclamation->SetRelativeLocation(m_winnerPoint.x, m_winnerPoint.y);
	m_pExclamation->SetTextureAsset(L"../Resource/느낌표.png", L"ExclamationTexture");
	m_pExclamation->AttachToComponent(m_pResultPanel);
	m_pExclamation->SetVisible(false);

	m_pWinnerTexture = CreateComponent<TextureComponent>(L"WinnerTexture");
	m_pWinnerTexture->SetRelativeLocation(0.f, 20.f);
	SetWinnerTextureAsset();
	m_pWinnerTexture->AttachToComponent(m_pResultPanel);
	m_pWinnerTexture->SetVisible(false);

	m_pExitButton = CreateComponent<ButtonUI>(L"ExitButton");
	m_pExitButton->SetWidth(300.f);
	m_pExitButton->SetHeight(110.f);
	m_pExitButton->SetColor(D2D1::ColorF::Red);
	m_pExitButton->SetYIndex(0);
	m_pExitButton->SetXIndex(0);
	m_pExitButton->SetRelativeLocation(ScreenWidth / 2.f, ScreenHeight / 2.f + 300.f);
	//m_pExitButton->SetButtonEventType(eEventType::ChangeWorldToMain);
	m_pExitButton->AttachToComponent(m_pRootPanel);

	m_DelayTime = 0.f;

	__super::Initialize();

	return true;
}

void InGameResultPopUp::Update(const float deltaTime)
{
	// Ending Before -> 부르릉(1초) -> Ending After -> (1초) -> WinnterTexture
	// TODO : 부르릉~ Sound
	if(!m_musicPlay1)
	{
		SoundManager::GetInstance()->StopMusic(eSoundChannel::BGM);
		SoundManager::GetInstance()->StopMusic(eSoundChannel::Player1State);
		SoundManager::GetInstance()->StopMusic(eSoundChannel::Player2State);
		SoundManager::GetInstance()->StopMusic(eSoundChannel::Player3State);
		SoundManager::GetInstance()->StopMusic(eSoundChannel::Player4State);
		SoundManager::GetInstance()->PlayMusic(eSoundList::TruckEffect, eSoundChannel::MapState, 1.f);
		m_musicPlay1 = true;
	}

	SetWinnerTextureAsset();

	m_DelayTime += deltaTime;
	if(m_DelayTime >= 2.f)
	{
		m_pEndingBeforeTexture->SetVisible(false);
		m_pEndingAfterTexture->SetVisible(true);
		m_pWinnerIdle->SetRelativeLocation(m_winnerPoint.x - ScreenWidth/2.f, m_winnerPoint.y - ScreenHeight/2.f);
		m_pExclamation->SetRelativeLocation(m_winnerPoint.x - ScreenWidth / 2.f, m_winnerPoint.y - ScreenHeight / 2.f - 60.f);
		m_pWinnerIdle->SetVisible(true);
		m_pExclamation->SetVisible(true);
	}
	if(m_DelayTime >= 5.f)
	{
		if(!m_musicPlay2)
		{
			SoundManager::GetInstance()->PlayMusic(eSoundList::WantedEffect, eSoundChannel::MapState, 1.f);
			m_musicPlay2 = true;
		}
		m_pEndingAfterTexture->SetVisible(false);
		m_pWinnerIdle->SetVisible(false);
		m_pExclamation->SetVisible(false);
		m_pWinnerTexture->SetVisible(true);

		if(InputManager::GetInstance()->IsPadButtonPush(0, GamePadButtonCode::Y))
		{
			EventManager::GetInstance()->SendEvent(eEventType::ChangeWorldToMain);
		}
	}

	__super::Update(deltaTime);
}

void InGameResultPopUp::SetWinnerTextureAsset()
{
	if(m_winner == 0)
	{
		m_pWinnerTexture->SetTextureAsset(L"../Resource/end_scene_Red_lighting.png", L"GameStartTexture");
	}
	else if (m_winner == 1)
	{
		m_pWinnerTexture->SetTextureAsset(L"../Resource/end_scene_Blue_lighting.png", L"GameStartTexture");
	}
	else if (m_winner == 2)
	{
		m_pWinnerTexture->SetTextureAsset(L"../Resource/end_scene_Yellow_lighting.png", L"GameStartTexture");
	}
	else if (m_winner == 3)
	{
		m_pWinnerTexture->SetTextureAsset(L"../Resource/end_scene_Green_lighting.png", L"GameStartTexture");
	}
}

void InGameResultPopUp::SetWinnerAnimtionAsset()
{
	std::vector<framework::FRAME_INFO> frame_IdleFront;
	frame_IdleFront.emplace_back(framework::FRAME_INFO({ 0, 110, 80, 220 }, 0.1f));
	frame_IdleFront.emplace_back(framework::FRAME_INFO({ 80, 110, 160, 220 }, 0.1f));
	frame_IdleFront.emplace_back(framework::FRAME_INFO({ 160, 110, 240, 220 }, 0.1f));
	frame_IdleFront.emplace_back(framework::FRAME_INFO({ 240, 110, 320, 220 }, 0.1f));

	if(m_winner == 0)
	{
		m_pWinnerIdle->SetAnimationAsset(L"../Resource/red_racoon_sheet.png", L"WINNER_IDLE", frame_IdleFront);
	}
	if(m_winner == 1)
	{
		m_pWinnerIdle->SetAnimationAsset(L"../Resource/blue_racoon_sheet.png", L"WINNER_IDLE", frame_IdleFront);
	}
	if (m_winner == 2)
	{
		m_pWinnerIdle->SetAnimationAsset(L"../Resource/yellow_racoon_sheet.png", L"WINNER_IDLE", frame_IdleFront);
	}
	if (m_winner == 3)
	{
		m_pWinnerIdle->SetAnimationAsset(L"../Resource/green_racoon_sheet.png", L"WINNER_IDLE", frame_IdleFront);
	}
	m_pWinnerIdle->SetAnimationName(L"WINNER_IDLE");
}
