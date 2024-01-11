#include "framework.h"
#include "MadeByWorld.h"

#include "MadeByUIObject.h"

#include "../Engine/Event.h"
#include "../Engine/EventManager.h"
#include "../Engine/WorldManager.h"

bool MadeByWorld::Initialize()
{
	m_pMainUIObject = CreateGameObject<MadeByUIObject>(L"MainUIObject", GROUP_TYPE::UI);

	bool res = __super::Initialize();
	assert(res);

	return true;
}

void MadeByWorld::Finalize()
{
	World::Finalize();
}


void MadeByWorld::Enter()
{
	std::cout << "MainWorld Enter" << std::endl;

	EventManager::GetInstance()->RegisterListener(eEventType::ChangeWorldToMain, dynamic_cast<EventListener*>(this));

	this->Initialize();
}

void MadeByWorld::Exit()
{
	std::cout << "MainWorld Exit" << std::endl;

	this->Finalize();
}

void MadeByWorld::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::ChangeWorldToMain)
	{
		m_pWorldManager->ChangeWorld(WORLD_TYPE::MAIN);
	}
}
