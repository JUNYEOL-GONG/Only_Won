#include "framework.h"
#include "MainWorld.h"

#include "MainUIObject.h"

#include "../Engine/Event.h"
#include "../Engine/EventManager.h"
#include "../Engine/WorldManager.h"
#include "../Engine/SoundManager.h"

bool MainWorld::Initialize()
{
	m_pMainUIObject = CreateGameObject<MainUIObject>(L"MainUIObject", GROUP_TYPE::UI);

	bool res = __super::Initialize();
	assert(res);

	return true;
}

void MainWorld::Finalize()
{
	World::Finalize();
}


void MainWorld::Enter()
{
	std::cout << "MainWorld Enter" << std::endl;

	EventManager::GetInstance()->RegisterListener(eEventType::ChangeWorldToGameSetting, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::ChangeWorldToInstruction, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::ChangeWorldToMadeBy, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::ChangeWorldToMadeBy, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::GameEnd, dynamic_cast<EventListener*>(this));

	SoundManager::GetInstance()->PlayMusic(eSoundList::TitleBGM, eSoundChannel::BGM, 0.6f);

	this->Initialize();
}

void MainWorld::Exit()
{
	std::cout << "MainWorld Exit" << std::endl;

	SoundManager::GetInstance()->StopMusic(eSoundChannel::BGM);
	this->Finalize();
}

void MainWorld::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::ChangeWorldToInstruction)
	{
		m_pWorldManager->ChangeWorld(WORLD_TYPE::INSTRUCTION);
	}
	else if (event->eventID == eEventType::ChangeWorldToMadeBy)
	{
		m_pWorldManager->ChangeWorld(WORLD_TYPE::MADEBY);
	}
	else if (event->eventID == eEventType::ChangeWorldToGameSetting)
	{
		m_pWorldManager->ChangeWorld(WORLD_TYPE::GAMESETTING);
	}
	else if (event->eventID == eEventType::GameEnd)
	{
		// 윈도우 종료
		PostQuitMessage(0);
	}
}