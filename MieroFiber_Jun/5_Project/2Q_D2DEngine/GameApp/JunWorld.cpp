#include "framework.h"
#include "JunWorld.h"

#include "PlayerObject.h"
#include "PlayerFSM1.h"
#include "PlayerFSM2.h"
#include "BoxObject.h"
#include "TrainSpawner.h"

#include "../Engine/BoxCollider2D.h"
#include "../Engine/CollisionManager.h"

#include <random>

#include "../Engine/Event.h"
#include "../Engine/EventManager.h"

JunWorld::JunWorld()
{
	SetWorldName(L"JunWorld");
}

JunWorld::~JunWorld()
{
	if (m_pPlayerFSM1 && m_pPlayerFSM2)
	{
		delete m_pPlayerFSM1;
		m_pPlayerFSM1 = nullptr;

		delete m_pPlayerFSM2;
		m_pPlayerFSM2 = nullptr;
	}
}

bool JunWorld::Initialize()
{
	// Test Random Code
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> triggerTimeDis(5, 7);
	std::uniform_int_distribution<> spawnTimeDis1(3, 4);
	std::uniform_int_distribution<> spawnTimeDis2(2, 3);
	std::uniform_int_distribution<> spawnTimeDis3(3, 7);

	// 오브젝트 생성
	m_pPlayer_1 = CreateGameObject<PlayerObject>(L"Player1", GROUP_TYPE::PLAYER);
	m_pPlayer_2 = CreateGameObject<PlayerObject>(L"Player2", GROUP_TYPE::PLAYER);

	m_pWall_1 = CreateGameObject<BoxObject>(L"Wall_1", GROUP_TYPE::Environment);
	m_pWall_2 = CreateGameObject<BoxObject>(L"Wall_2", GROUP_TYPE::Environment);
	m_pWall_3 = CreateGameObject<BoxObject>(L"Wall_3", GROUP_TYPE::Environment);
	m_pWall_4 = CreateGameObject<BoxObject>(L"Wall_4", GROUP_TYPE::Environment);

	m_pTrainSpawner_1 = CreateGameObject<TrainSpawner>(L"TrainSpawner_1", GROUP_TYPE::SPAWNER);
	m_pTrainSpawner_2 = CreateGameObject<TrainSpawner>(L"TrainSpawner_2", GROUP_TYPE::SPAWNER);
	m_pTrainSpawner_3 = CreateGameObject<TrainSpawner>(L"TrainSpawner_3", GROUP_TYPE::SPAWNER);

	// 모든 오브젝트 초기화 (컴포넌트 부착 타이밍)
	const bool res = World::Initialize();
	assert(res);

	// Player FSM Attach
	m_pPlayerFSM1 = new PlayerFSM1;
	m_pPlayerFSM2 = new PlayerFSM2;
	m_pPlayer_1->AttachFSM(m_pPlayerFSM1);
	m_pPlayer_2->AttachFSM(m_pPlayerFSM2);

	// 플레이어 위치 설정
	m_pPlayer_1->SetGameObjectLocation(ScreenWidth / 2.f - 500.f, ScreenHeight / 2.f);
	m_pPlayer_2->SetGameObjectLocation(ScreenWidth / 2.f + 500.f, ScreenHeight / 2.f);

	// 벽의 위치, 충돌체 크기 설정
	m_pWall_1->SetGameObjectLocation(ScreenWidth / 2.f, 0.f);
	dynamic_cast<BoxCollider2D*>(m_pWall_1->GetComponent<Collider2D>())->SetExtend(1000.f, 50.f);

	m_pWall_2->SetGameObjectLocation(ScreenWidth / 2.f, ScreenHeight);
	dynamic_cast<BoxCollider2D*>(m_pWall_2->GetComponent<Collider2D>())->SetExtend(1000.f, 50.f);

	m_pWall_3->SetGameObjectLocation(0.f, ScreenHeight / 2.f);
	dynamic_cast<BoxCollider2D*>(m_pWall_3->GetComponent<Collider2D>())->SetExtend(50.f, 1000.f);

	m_pWall_4->SetGameObjectLocation(ScreenWidth, ScreenHeight / 2.f);
	dynamic_cast<BoxCollider2D*>(m_pWall_4->GetComponent<Collider2D>())->SetExtend(50.f, 1000.f);

	// 기차 생성기 설정
	m_pTrainSpawner_1->AddSpawnPosition(framework::Vector2D(ScreenWidth / 2.f - 350.f, -1000.f));
	m_pTrainSpawner_1->AddSpawnPosition(framework::Vector2D(ScreenWidth / 2.f - 350.f, ScreenHeight + 1000.f));
	//m_pTrainSpawner_1->SetTriggerTime(static_cast<float>(triggerTimeDis(gen)));
	//m_pTrainSpawner_1->SetSpawnTime(static_cast<float>(spawnTimeDis1(gen)));
	m_pTrainSpawner_1->SetTriggerTime(1);
	m_pTrainSpawner_1->SetSpawnTime(1);

	m_pTrainSpawner_2->AddSpawnPosition(framework::Vector2D(ScreenWidth / 2.f - 0, -1000.f));
	m_pTrainSpawner_2->AddSpawnPosition(framework::Vector2D(ScreenWidth / 2.f - 0, ScreenHeight + 1000.f));
	//m_pTrainSpawner_2->SetTriggerTime(static_cast<float>(triggerTimeDis(gen)));
	//m_pTrainSpawner_2->SetSpawnTime(static_cast<float>(spawnTimeDis2(gen)));
	m_pTrainSpawner_2->SetTriggerTime(1);
	m_pTrainSpawner_2->SetSpawnTime(1);

	m_pTrainSpawner_3->AddSpawnPosition(framework::Vector2D(ScreenWidth / 2.f + 350.f, -1000.f));
	m_pTrainSpawner_3->AddSpawnPosition(framework::Vector2D(ScreenWidth / 2.f + 350.f, ScreenHeight + 1000.f));
	//m_pTrainSpawner_3->SetTriggerTime(static_cast<float>(triggerTimeDis(gen)));
	//m_pTrainSpawner_3->SetSpawnTime(static_cast<float>(spawnTimeDis3(gen)));
	m_pTrainSpawner_3->SetTriggerTime(1);
	m_pTrainSpawner_3->SetSpawnTime(1);

	// World 내의 충돌 Matrix 설정
	m_pCollisionManager->EnableCollisionType(GROUP_TYPE::PLAYER, GROUP_TYPE::ITEM);
	m_pCollisionManager->EnableCollisionType(GROUP_TYPE::PLAYER, GROUP_TYPE::Environment);

	return true;
}

void JunWorld::Update(const float deltaTime)
{
	World::Update(deltaTime);
}

void JunWorld::Finalize()
{
	if (m_pPlayerFSM1 && m_pPlayerFSM2)
	{
		delete m_pPlayerFSM1;
		m_pPlayerFSM1 = nullptr;

		delete m_pPlayerFSM2;
		m_pPlayerFSM2 = nullptr;
	}

	World::Finalize();
}

void JunWorld::Enter()
{
	std::cout << "JunWorld Enter" << std::endl;

	// DeleteObject 이벤트 처리할 월드 Listener 등록
	EventManager::GetInstance()->RegisterListener(eEventType::DeleteGameObject, dynamic_cast<EventListener*>(this));

	this->Initialize();
}

void JunWorld::Exit()
{
	std::cout << "JunWorld Exit" << std::endl;

	this->Finalize();
}

void JunWorld::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::DeleteGameObject)
	{
		DeleteGameObject(event->objGroup, event->obj);
	}
}