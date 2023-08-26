#include "framework.h"
#include "GameSettingWorld.h"

#include "GameSettingUIObject.h"

#include "../Engine/Event.h"
#include "../Engine/EventManager.h"
#include "../Engine/SoundManager.h"
#include "../Engine/WorldManager.h"

bool GameSettingWorld::Initialize()
{
	m_pGameSettingUIObject = CreateGameObject<GameSettingUIObject>(L"MainUIObject", GROUP_TYPE::UI);

	bool res = __super::Initialize();
	assert(res);

	return true;
}

void GameSettingWorld::Finalize()
{
	World::Finalize();
}


void GameSettingWorld::Enter()
{
	std::cout << "GameSettingWorld Enter" << std::endl;

	EventManager::GetInstance()->RegisterListener(eEventType::ChangeWorldToInGame, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::ChangeWorldToMain, dynamic_cast<EventListener*>(this));

	SoundManager::GetInstance()->PlayMusic(eSoundList::GameSettingBGM, eSoundChannel::BGM, 0.6f);

	this->Initialize();
}

void GameSettingWorld::Exit()
{
	std::cout << "GameSettingWorld Exit" << std::endl;

	SoundManager::GetInstance()->StopMusic(eSoundChannel::BGM);

	this->Finalize();
}

void GameSettingWorld::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::ChangeWorldToInGame)
	{
		m_pWorldManager->ChangeWorld(WORLD_TYPE::INGAME);
	}
	if (event->eventID == eEventType::ChangeWorldToInGame2)
	{
		//m_pWorldManager->ChangeWorld(WORLD_TYPE::INGAME);
	}
	if (event->eventID == eEventType::ChangeWorldToMain)
	{
		m_pWorldManager->ChangeWorld(WORLD_TYPE::MAIN);
	}
}