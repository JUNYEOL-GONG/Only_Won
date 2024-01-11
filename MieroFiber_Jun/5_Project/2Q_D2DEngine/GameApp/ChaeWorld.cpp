#include "framework.h"
#include "ChaeWorld.h"

#include "UIObject.h"
#include "PopUpUIObject.h"
#include "PopUpUIObject2.h"
#include "PlayerObject1.h"

#include "../Engine/Event.h"
#include "../Engine/EventManager.h"
#include "../Engine/WorldManager.h"

ChaeWorld::ChaeWorld()
{
	SetWorldName(L"ChaeWorld");
}

bool ChaeWorld::Initialize()
{
	m_pUIObject = CreateGameObject<UIObject>(L"MainUI", GROUP_TYPE::UI);
	m_pUIObject->SetActiveObj(true);

	m_pPopUpUIObject1 = CreateGameObject<PopUpUIObject>(L"PopUpUI", GROUP_TYPE::POPUPUI);
	m_pPopUpUIObject1->SetActiveObj(false);
	m_pPopUpUIObject1->SetPopUpUIEventType(eEventType::CheckQuitGame);

	m_pPopUpUIObject2 = CreateGameObject<PopUpUIObject2>(L"PopUpUI", GROUP_TYPE::POPUPUI);
	m_pPopUpUIObject2->SetActiveObj(false);
	//m_pPopUpUIObject2->SetPopUpUIEventType(eEventType::RUOkay);

	m_pPlayerObject1 = CreateGameObject<PlayerObject1>(L"Player1", GROUP_TYPE::PLAYER);

	bool res = __super::Initialize();
	assert(res);

	return true;
}

void ChaeWorld::Finalize()
{
	World::Finalize();
}

void ChaeWorld::Enter()
{
	// std::cout << "ChaeWorld Enter" << std::endl;

	EventManager::GetInstance()->RegisterListener(eEventType::ChangeWorld, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::PauseOn, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::PauseOff, dynamic_cast<EventListener*>(this));
	//EventManager::GetInstance()->RegisterListener(eEventType::RUOkay, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::CheckQuitGame, dynamic_cast<EventListener*>(this));
	//EventManager::GetInstance()->RegisterListener(eEventType::Okay, dynamic_cast<EventListener*>(this));
	//EventManager::GetInstance()->RegisterListener(eEventType::No, dynamic_cast<EventListener*>(this));

	this->Initialize();
}

void ChaeWorld::Exit()
{
	// std::cout << "ChaeWorld Exit" << std::endl;

	this->Finalize();
}

void ChaeWorld::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::ChangeWorld)
	{
		m_pWorldManager->ChangeWorld(WORLD_TYPE::JUN);
		m_pPopUpUIObject2->SetPopUpState(false);
		m_pPopUpUIObject1->SetPopUpState(false);
	}
	else if (event->eventID == eEventType::PauseOn)
	{
		this->SetWorldPauseState(true);
		// printf("못 움직인다.");
	}
	else if (event->eventID == eEventType::PauseOff)
	{
		this->SetWorldPauseState(false);
		// printf("움직인다.");
		m_pPopUpUIObject2->SetPopUpState(false);
		m_pPopUpUIObject1->SetPopUpState(false);
		m_pPopUpUIObject1->SetActiveObj(false);
		m_pPopUpUIObject2->SetActiveObj(false);
	}
	/*else if (event->eventID == eEventType::RUOkay)
	{
		this->SetWorldPauseState(true);
		m_pPopUpUIObject2->SetActiveObj(true);
		m_pPopUpUIObject2->SetPopUpState(true);
	}
	else if (event->eventID == eEventType::CheckQuitGame)
	{
		this->SetWorldPauseState(true);
		m_pPopUpUIObject1->SetActiveObj(true);
		m_pPopUpUIObject1->SetPopUpState(true);
	}
	else if (event->eventID == eEventType::Okay)
	{
		printf("Okay\n");
		this->SetWorldPauseState(false);
		m_pPopUpUIObject2->SetActiveObj(false);
		m_pPopUpUIObject2->SetPopUpState(false);
	}
	else if (event->eventID == eEventType::No)
	{
		printf("No\n");
		this->SetWorldPauseState(false);
		m_pPopUpUIObject2->SetActiveObj(false);
		m_pPopUpUIObject2->SetPopUpState(false);
	}*/
}