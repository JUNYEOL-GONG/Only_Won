#include "pch.h"
#include "WorldManager.h"

#include "UIManager.h"
#include "EventManager.h"
#include "InputManager.h"
#include "SoundManager.h"

WorldManager::WorldManager()
	: m_currentWorld(nullptr)
	, m_prevWorld(nullptr)
{
	// World의 개수만큼 확보
	m_worlds.resize(static_cast<UINT>(WORLD_TYPE::END));

	m_pUIManager = new UIManager;
}

void WorldManager::Initialize()
{
	// Set Prev World
	m_prevWorld = m_currentWorld;

	// Enter to Current World
	m_currentWorld->Enter();

	m_pUIManager->SetWorldPopUpUIObject(m_currentWorld->GetGameObjectGroup(GROUP_TYPE::POPUPUI));
	m_pUIManager->SetWorldUIObject(m_currentWorld->GetGameObjectGroup(GROUP_TYPE::UI));
}

void WorldManager::Update(float deltaTime)
{

	#ifdef _DEBUG
	if(InputManager::GetInstance()->IsKeyPush(ESCAPE))
	{
		// 윈도우 종료
		PostQuitMessage(0);
	}
	#endif

	/// todo(채원)
	/// start해서 일시정지하는건 게임 월드에서만 해줘도 되기 때문에 여기서 하지말자
	if(m_currentWorld->GetWorldName() == L"InGameWorld")
	{
		// 정지 되어있으면 true
		m_bPaused = m_currentWorld->GetWorldPauseState();
		if (m_bPaused)
		{
			//PopUpUI 오브젝트만 업데이트
			for (auto& curPopUpUIObject : m_currentWorld->GetGameObjectGroup(GROUP_TYPE::POPUPUI))
			{
				if (curPopUpUIObject->IsObjActive())
					curPopUpUIObject->Update(deltaTime);
			}
			m_pUIManager->Update(deltaTime);
			m_bPaused = false;
			return;
		}
	}
	else
	{
		if (InputManager::GetInstance()->IsPadButtonPush(0, GamePadButtonCode::B))
		{
			SoundManager::GetInstance()->PlayMusic(eSoundList::UICancleEffect, eSoundChannel::Player1State, 1.f);
		}
		else if (InputManager::GetInstance()->IsPadButtonPush(0, GamePadButtonCode::A))
		{
			SoundManager::GetInstance()->PlayMusic(eSoundList::UIClickEffect, eSoundChannel::Player1State, 1.f);
		}
	}

	m_currentWorld->Update(deltaTime);
	m_pUIManager->Update(deltaTime);
}

void WorldManager::Render()
{
	m_currentWorld->Render();
}

void WorldManager::ChangeWorld(WORLD_TYPE world)
{
	// Exit from Current World
	/// World Exit 전에 이벤트 등록 내용 Clear
	EventManager::GetInstance()->ClearEvents();
	m_currentWorld->Exit();

	// Set Prev World
	m_prevWorld = m_currentWorld;

	// Set Next World
	m_currentWorld = m_worlds[static_cast<UINT>(world)];

	// Enter to Next World
	/// World Enter 후에 UIManager Init, EventManager Init 필요하다.
	///	UI, Event는 World에 종속적이기 때문
	m_currentWorld->Enter();

	//이전에 퍼즈 상태였다면 풀어주기
	m_currentWorld->SetWorldPauseState(false);

	// 그 전의 오브젝트를 비워줘야함.
	m_pUIManager->ClearUIObject();
	m_pUIManager->SetWorldUIObject(m_currentWorld->GetGameObjectGroup(GROUP_TYPE::UI));
	m_pUIManager->SetWorldPopUpUIObject(m_currentWorld->GetGameObjectGroup(GROUP_TYPE::POPUPUI));
	EventManager::GetInstance()->Initialize();
}

void WorldManager::GoToNextWorld()
{
	const UINT curWorldID = m_currentWorld->GetID();
	const UINT nextWorldID = (curWorldID + 1) % m_worlds.size();

	// std::cout << curWorldID << std::endl;
	// std::cout << nextWorldID << std::endl;

	// Set Prev World
	m_prevWorld = m_currentWorld;

	// Exit from Current World
	EventManager::GetInstance()->ClearEvents();
	m_currentWorld->Exit();

	// Set Next World
	m_currentWorld = m_worlds[nextWorldID];

	// Enter to Next World
	m_currentWorld->Enter();
	m_pUIManager->SetWorldUIObject(m_currentWorld->GetGameObjectGroup(GROUP_TYPE::UI));
	EventManager::GetInstance()->Initialize();
}

void WorldManager::GoToPrevWorld()
{
	const UINT curWorldID = m_currentWorld->GetID();
	const UINT prevWorldID = (curWorldID - 1 + m_worlds.size()) % static_cast<UINT>(m_worlds.size());

	// std::cout << curWorldID << std::endl;
	// std::cout << prevWorldID << std::endl;

	// Set Prev World
	m_prevWorld = m_currentWorld;

	// Exit from Current World
	EventManager::GetInstance()->ClearEvents();
	m_currentWorld->Exit();

	// Set Next World
	m_currentWorld = m_worlds[prevWorldID];

	// Enter to Next World
	m_currentWorld->Enter();
	m_pUIManager->SetWorldUIObject(m_currentWorld->GetGameObjectGroup(GROUP_TYPE::UI));
	EventManager::GetInstance()->Initialize();
}

void WorldManager::Finalize()
{
	// Delete World
	for (auto& world : m_worlds)
	{
		if (world != nullptr)
		{
			delete world;
			world = nullptr;
		}
	}

	delete m_pUIManager;
	delete this;
}

void WorldManager::RegisterWorld(World* world, WORLD_TYPE type)
{
	world->SetID(static_cast<UINT>(type));
	m_worlds[static_cast<UINT>(type)] = world;

	world->SetWorldManager(this);
}