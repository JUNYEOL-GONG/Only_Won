#include "framework.h"
#include "InGamePopUpUI2.h"

#include "../Engine/EventManager.h"
#include "../Engine/PanelUI.h"
#include "../Engine/SoundManager.h"
#include "../Engine/TextureComponent.h"
#include "../Engine/TimeManager.h"

bool InGamePopUpUI2::Initialize()
{
	m_elapsedTime = 0.f;
	m_playTime1 = false;
	m_playTime2 = false;

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
	m_pPopUpPanel->AttachToComponent(m_pRootPanel);

	m_pBackground2 = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pBackground2->SetRelativeLocation(ScreenWidth / 2.f, ScreenHeight / 2.f);
	m_pBackground2->SetTextureAsset(L"../Resource/EndingBefore.png", L"GameStartTexture");
	m_pBackground2->SetShowTexture(false);
	m_pBackground2->AttachToComponent(m_pPopUpPanel);

	m_pReadyGameStart = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pReadyGameStart->SetRelativeLocation(ScreenWidth / 2.f, ScreenHeight / 2.f);
	m_pReadyGameStart->SetTextureAsset(L"../Resource/Ingame_Ready.png", L"GameStartTexture");
	m_pReadyGameStart->SetShowTexture(false);
	m_pReadyGameStart->AttachToComponent(m_pPopUpPanel);

	m_pBackground1 = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pBackground1->SetRelativeLocation(ScreenWidth / 2.f, ScreenHeight / 2.f);
	m_pBackground1->SetTextureAsset(L"../Resource/WhiteBackground.png", L"GameStartTexture");
	m_pBackground1->SetShowTexture(false);
	m_pBackground1->AttachToComponent(m_pPopUpPanel);

	m_pCutScene = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pCutScene->SetRelativeLocation(ScreenWidth / 2.f, ScreenHeight / 2.f);
	m_pCutScene->SetTextureAsset(L"../Resource/scene_start_01.png", L"GameStartTexture");
	m_pCutScene->SetShowTexture(false);
	m_pCutScene->AttachToComponent(m_pPopUpPanel);

	__super::Initialize();

	return true;
}

void InGamePopUpUI2::Update(const float deltaTime)
{
//#ifdef _DEBUG
//	return;
//#endif
	m_elapsedTime += deltaTime;

	if (m_elapsedTime >= 0.f && m_elapsedTime < 1.5f)
	{
		EventManager::GetInstance()->SendEvent(eEventType::PauseOn);
		m_pCutScene->SetShowTexture(true);
		m_pBackground1->SetShowTexture(true);
	}
	else if (m_elapsedTime >= 1.5f && m_elapsedTime < 3.f)
	{
		m_pCutScene->SetTextureAsset(L"../Resource/scene_start_02.png", L"GameStartTexture");
	}
	else if (m_elapsedTime >= 3.f && m_elapsedTime < 4.5f)
	{
		m_pCutScene->SetTextureAsset(L"../Resource/scene_start_03.png", L"GameStartTexture");
	}
	else if (m_elapsedTime >= 4.5f && m_elapsedTime < 6.f)
	{
		m_pCutScene->SetTextureAsset(L"../Resource/scene_start_04.png", L"GameStartTexture");
	}
	else if (m_elapsedTime >= 6.f && m_elapsedTime < 8.5f)
	{
		m_pCutScene->SetTextureAsset(L"../Resource/scene_start_all.png", L"GameStartTexture");
	}
	else if(m_elapsedTime >= 8.5f && m_elapsedTime < 10.5f)
	{
		if (!m_playTime1)
		{
			SoundManager::GetInstance()->PlayMusic(eSoundList::ReadyEffect, eSoundChannel::ReadySetGo, 1.f);
			m_playTime1 = true;
		}
		m_pBackground1->SetShowTexture(false);
		m_pCutScene->SetShowTexture(false);
		m_pReadyGameStart->SetShowTexture(true);
		m_pBackground2->SetShowTexture(true);
	}
	else if(m_elapsedTime >= 10.5f && m_elapsedTime < 12.5f)
	{
		if (!m_playTime2)
		{
			SoundManager::GetInstance()->PlayMusic(eSoundList::GoEffect, eSoundChannel::ReadySetGo, 1.f);
			m_playTime2 = true;
		}
		m_pReadyGameStart->SetTextureAsset(L"../Resource/Ingame_Start.png", L"GameStartTexture");
	}
	else
	{
		m_pReadyGameStart->SetShowTexture(false);
		m_pBackground2->SetShowTexture(false);
		this->SetActiveObj(false);
		EventManager::GetInstance()->SendEvent(eEventType::PauseOff);
	}

	__super::Update(deltaTime);
}
