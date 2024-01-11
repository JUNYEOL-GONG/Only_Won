#include "framework.h"
#include "InGameWorld.h"

#include <random>

#include "Background.h"
#include "ObstacleObject.h"
#include "TrafficObject.h"
#include "TrainSpawner.h"
#include "ItemBoxObject.h"
#include "ItemObject.h"
#include "InGameUIObject.h"
#include "PlayerObject.h"
#include "MoneyObject.h"
#include "MoneySpawner.h"
#include "ItemSpawner.h"
#include "PlayerFSM1.h"
#include "PlayerFSM2.h"
#include "PlayerFSM3.h"
#include "PlayerFSM4.h"
#include "Bar1Object.h"
#include "Bar2Object.h"
#include "Bar3Object.h"
#include "Bar4Object.h"
#include "InGamePopUpUI1.h"
#include "InGamePopUpUI2.h"
#include "InGameResultPopUp.h"

#include "../Engine/CollisionManager.h"
#include "../Engine/Event.h"
#include "../Engine/EventManager.h"
#include "../Engine/WorldManager.h"
#include "../Engine/SoundManager.h"

InGameWorld::~InGameWorld()
{
	if(m_pPlayerFSM1 && m_pPlayerFSM2 && m_pPlayerFSM3 && m_pPlayerFSM4)
	{
		delete m_pPlayerFSM1;
		m_pPlayerFSM1 = nullptr;
		delete m_pPlayerFSM2;
		m_pPlayerFSM2 = nullptr;
		delete m_pPlayerFSM3;
		m_pPlayerFSM3 = nullptr;
		delete m_pPlayerFSM4;
		m_pPlayerFSM4 = nullptr;
	}
}

bool InGameWorld::Initialize()
{
	m_WorldName = L"InGameWorld";

	EventManager::GetInstance()->RegisterListener(eEventType::PauseOn, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::PauseOff, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::CheckQuitGame, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::Continue, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::ChangeWorldToMain, dynamic_cast<EventListener*>(this));

	// Test Random Code
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> triggerTimeDis1(10, 20);
	std::uniform_int_distribution<> triggerTimeDis2(20, 30);
	std::uniform_int_distribution<> triggerTimeDis3(10, 20);
	std::uniform_int_distribution<> spawnTimeDis(2, 5);

	/// Background
	m_pBackground = CreateGameObject<Background>(L"Background", GROUP_TYPE::BACKGROUND);

	/// ObstacleObject
	m_pTunnel_1 = CreateGameObject<ObstacleObject>(L"Tunnel_1", GROUP_TYPE::Environment);
	m_pTunnel_2 = CreateGameObject<ObstacleObject>(L"Tunnel_2", GROUP_TYPE::Environment);
	m_pTunnel_3 = CreateGameObject<ObstacleObject>(L"Tunnel_3", GROUP_TYPE::Environment);
	m_pTunnel_4 = CreateGameObject<ObstacleObject>(L"Tunnel_4", GROUP_TYPE::Environment);
	m_pTunnel_5 = CreateGameObject<ObstacleObject>(L"Tunnel_5", GROUP_TYPE::Environment);
	m_pTunnel_6 = CreateGameObject<ObstacleObject>(L"Tunnel_6", GROUP_TYPE::Environment);

	m_pBarrel_1 = CreateGameObject<ObstacleObject>(L"Barrel", GROUP_TYPE::Environment);
	m_pBarrel_2 = CreateGameObject<ObstacleObject>(L"Barrel", GROUP_TYPE::Environment);
	m_pBarrel_3 = CreateGameObject<ObstacleObject>(L"Barrel", GROUP_TYPE::Environment);
	m_pBarrel_4 = CreateGameObject<ObstacleObject>(L"Barrel", GROUP_TYPE::Environment);
	m_pBarrel_5 = CreateGameObject<ObstacleObject>(L"Barrel", GROUP_TYPE::OBSTACLE);
	m_pBarrel_6 = CreateGameObject<ObstacleObject>(L"Barrel", GROUP_TYPE::OBSTACLE);

	m_pBox = CreateGameObject<ObstacleObject>(L"Box", GROUP_TYPE::Environment);

	m_pTraffic_1 = CreateGameObject<TrafficObject>(L"Traffic_1", GROUP_TYPE::Environment);
	m_pTraffic_2 = CreateGameObject<TrafficObject>(L"Traffic_2", GROUP_TYPE::Environment);
	m_pTraffic_3 = CreateGameObject<TrafficObject>(L"Traffic_3", GROUP_TYPE::Environment);

	m_pTruck = CreateGameObject<ObstacleObject>(L"Truck", GROUP_TYPE::Environment);

	/// TrainSpawner
	m_pTrainSpawner_1 = CreateGameObject<TrainSpawner>(L"TrainSpawner_1", GROUP_TYPE::SPAWNER);
	m_pTrainSpawner_2 = CreateGameObject<TrainSpawner>(L"TrainSpawner_2", GROUP_TYPE::SPAWNER);
	m_pTrainSpawner_3 = CreateGameObject<TrainSpawner>(L"TrainSpawner_3", GROUP_TYPE::SPAWNER);
	m_pTrainSpawner_1->LinkTrafficObject(m_pTraffic_1);
	m_pTrainSpawner_2->LinkTrafficObject(m_pTraffic_2);
	m_pTrainSpawner_3->LinkTrafficObject(m_pTraffic_3);

	/// Player
	m_pPlayerObject1 = CreateGameObject<PlayerObject>(L"Player1", GROUP_TYPE::PLAYER);
	m_pPlayerObject2 = CreateGameObject<PlayerObject>(L"Player2", GROUP_TYPE::PLAYER);
	m_pPlayerObject3 = CreateGameObject<PlayerObject>(L"Player3", GROUP_TYPE::PLAYER);
	m_pPlayerObject4 = CreateGameObject<PlayerObject>(L"Player4", GROUP_TYPE::PLAYER);
	m_PlayerList.emplace_back(m_pPlayerObject1);
	m_PlayerList.emplace_back(m_pPlayerObject2);
	m_PlayerList.emplace_back(m_pPlayerObject3);
	m_PlayerList.emplace_back(m_pPlayerObject4);

	m_pInGameUIObject = CreateGameObject<InGameUIObject>(L"MainUIObject", GROUP_TYPE::UI);

	// 일시정지 팝업
	m_pInGamePopUpUI1 = CreateGameObject<InGamePopUpUI1>(L"PopUpUI1", GROUP_TYPE::POPUPUI);
	m_pInGamePopUpUI1->SetActiveObj(false);
	m_pInGamePopUpUI1->SetPopUpUIEventType(eEventType::CheckQuitGame);

	// todo: 이거 true로 바꿔줘야함
	// 준비됐나요? 게임 시작 팝업
	m_pInGamePopUpUI2 = CreateGameObject<InGamePopUpUI2>(L"PopUpUI2", GROUP_TYPE::POPUPUI);
	m_pInGamePopUpUI2->SetActiveObj(true);

	// 게임 결과창 팝업
	m_pInGameResultPopUp = CreateGameObject<InGameResultPopUp>(L"PopUpUI3", GROUP_TYPE::POPUPUI);
	m_pInGameResultPopUp->SetActiveObj(false);
	m_pInGameResultPopUp->SetPopUpState(false);

	m_pMoneySpawner = CreateGameObject<MoneySpawner>(L"Spawner", GROUP_TYPE::SPAWNER);
	m_pMoneySpawner->SetType(eMoneyType::SPAWNED);
	m_pMoneySpawner->SetItemSpawnArea({190, 620, 500, 880});
	m_pMoneySpawner->SetItemSpawnArea({400, 960, 500, 1040});
	m_pMoneySpawner->SetItemCount(10);
	m_pMoneySpawner->SetScale(30);
	m_pMoneySpawner->SetItemSpawnTime(20.f);

	m_pItemThrowSpawner = CreateGameObject<ItemSpawner>(L"Spawner", GROUP_TYPE::SPAWNER);
	m_pItemThrowSpawner->SetType(eItemBoxType::THROW);
	m_pItemThrowSpawner->SetItemSpawnArea({ 685, 120, 1490, 290 });
	m_pItemThrowSpawner->SetItemSpawnArea({ 1265, 370,1490, 570 });
	m_pItemThrowSpawner->SetItemSpawnArea({ 950, 650,1490, 880 });
	m_pItemThrowSpawner->SetItemCount(4);
	m_pItemThrowSpawner->SetScale(40);
	m_pItemThrowSpawner->SetItemSpawnTime(7.f);

	m_pItemInstallationSpawner = CreateGameObject<ItemSpawner>(L"Spawner", GROUP_TYPE::SPAWNER);
	m_pItemInstallationSpawner->SetType(eItemBoxType::INSTALLATION);
	m_pItemInstallationSpawner->SetItemSpawnArea({ 940, 120, 1185, 880 });
	m_pItemInstallationSpawner->SetItemSpawnArea({ 1265, 370, 1490, 570 });
	m_pItemInstallationSpawner->SetItemCount(3);
	m_pItemInstallationSpawner->SetScale(40);
	m_pItemInstallationSpawner->SetItemSpawnTime(12.f);

	m_pItemReinforcedSpawner = CreateGameObject<ItemSpawner>(L"Spawner", GROUP_TYPE::SPAWNER);
	m_pItemReinforcedSpawner->SetType(eItemBoxType::REINFORCED);
	m_pItemReinforcedSpawner->SetItemSpawnArea({ 1185, 120, 1490, 570 });
	m_pItemReinforcedSpawner->SetItemSpawnArea({ 685, 650,870, 880 });
	m_pItemReinforcedSpawner->SetItemCount(3);
	m_pItemReinforcedSpawner->SetScale(40);
	m_pItemReinforcedSpawner->SetItemSpawnTime(12.f);

	m_pBar1Object = CreateGameObject<Bar1Object>(L"Bar1", GROUP_TYPE::BARUI);
	m_pBar2Object = CreateGameObject<Bar2Object>(L"Bar2", GROUP_TYPE::BARUI);
	m_pBar3Object = CreateGameObject<Bar3Object>(L"Bar3", GROUP_TYPE::BARUI);
	m_pBar4Object = CreateGameObject<Bar4Object>(L"Bar4", GROUP_TYPE::BARUI);

	m_pBar1Object->SetOwnerObj(m_pPlayerObject1);
	m_pBar2Object->SetOwnerObj(m_pPlayerObject2);
	m_pBar3Object->SetOwnerObj(m_pPlayerObject3);
	m_pBar4Object->SetOwnerObj(m_pPlayerObject4);

	/// 월드 초기화
	const bool res = World::Initialize();
	assert(res);

	// Obstacle Object Init
	m_pTunnel_1->SetTexture(L"../Resource/Object_Cave_entrance.png", L"Tunnel_1");
	m_pTunnel_1->SetObstaclePos(741.f, 27.f);
	m_pTunnel_1->SetObstacleRotation(180.f);
	m_pTunnel_1->SetColliderExtend(146.f, 27.f);

	m_pTunnel_2->SetTexture(L"../Resource/Object_Cave_entrance.png", L"Tunnel_2");
	m_pTunnel_2->SetObstaclePos(1061.f, 27.f);
	m_pTunnel_2->SetObstacleRotation(180.f);
	m_pTunnel_2->SetColliderExtend(146.f, 27.f);

	m_pTunnel_3->SetTexture(L"../Resource/Object_Cave_entrance.png", L"Tunnel_3");
	m_pTunnel_3->SetObstaclePos(1381.f, 27.f);
	m_pTunnel_3->SetObstacleRotation(180.f);
	m_pTunnel_3->SetColliderExtend(146.f, 27.f);

	m_pTunnel_4->SetTexture(L"../Resource/Object_Cave_entrance.png", L"Tunnel_4");
	m_pTunnel_4->SetObstaclePos(741.f, 1055.f);
	m_pTunnel_4->SetColliderExtend(146.f, 27.f);

	m_pTunnel_5->SetTexture(L"../Resource/Object_Cave_entrance.png", L"Tunnel_5");
	m_pTunnel_5->SetObstaclePos(1061.f, 1055.f);
	m_pTunnel_5->SetColliderExtend(146.f, 27.f);

	m_pTunnel_6->SetTexture(L"../Resource/Object_Cave_entrance.png", L"Tunnel_6");
	m_pTunnel_6->SetObstaclePos(1381.f, 1055.f);
	m_pTunnel_6->SetColliderExtend(146.f, 27.f);

	m_pBarrel_1->SetTexture(L"../Resource/wood_barrel.png", L"Barrel");
	m_pBarrel_1->SetObstaclePos(212.5f, 369.f);
	m_pBarrel_1->SetColliderExtend(47.5f, 49.f);

	m_pBarrel_2->SetTexture(L"../Resource/wood_barrel.png", L"Barre2");
	m_pBarrel_2->SetObstaclePos(312.5f, 379.f);
	m_pBarrel_2->SetColliderExtend(47.5f, 49.f);

	m_pBarrel_3->SetTexture(L"../Resource/wood_barrel.png", L"Barre3");
	m_pBarrel_3->SetObstaclePos(262.5f, 459.f);
	m_pBarrel_3->SetColliderExtend(47.5f, 49.f);

	m_pBarrel_4->SetTexture(L"../Resource/wood_barrel.png", L"Barre4");
	m_pBarrel_4->SetObstaclePos(597.5f, 444.f);
	m_pBarrel_4->SetColliderExtend(47.5f, 49.f);

	m_pBarrel_5->SetTexture(L"../Resource/wood_barrel.png", L"Barre5");
	m_pBarrel_5->SetObstaclePos(1582.5f, 259.f);
	m_pBarrel_5->SetColliderExtend(47.5f, 49.f);

	m_pBarrel_6->SetTexture(L"../Resource/wood_barrel.png", L"Barre6");
	m_pBarrel_6->SetObstaclePos(1582.5f + 100.f, 259.f - 50.f);
	m_pBarrel_6->SetColliderExtend(47.5f, 49.f);

	m_pBox->SetTexture(L"../Resource/wood_box.png", L"Box");
	m_pBox->SetObstaclePos(432.f, 454.f);
	m_pBox->SetColliderPos(-59.f, -15.f);
	m_pBox->SetColliderExtend(280.f, 125.f);

	m_pTraffic_1->SetTexture(L"../Resource/Object_Train_TrafficLight.png", L"Traffic_1");
	m_pTraffic_1->SetObstaclePos(845.5, 188.5);
	m_pTraffic_1->SetColliderPos(0.f, 70.f);
	m_pTraffic_1->SetColliderExtend(30.5f, 25.f);

	m_pTraffic_2->SetTexture(L"../Resource/Object_Train_TrafficLight.png", L"Traffic_2");
	m_pTraffic_2->SetObstaclePos(1160.5, 478.5);
	m_pTraffic_2->SetColliderPos(0.f, 70.f);
	m_pTraffic_2->SetColliderExtend(30.5f, 25.f);

	m_pTraffic_3->SetTexture(L"../Resource/Object_Train_TrafficLight.png", L"Traffic_3");
	m_pTraffic_3->SetObstaclePos(1480.5, 903.5);
	m_pTraffic_3->SetColliderPos(0.f, 70.f);
	m_pTraffic_3->SetColliderExtend(30.5f, 25.f);

	m_pTruck->SetTexture(L"../Resource/Object_Truck.png", L"Truck");
	m_pTruck->SetObstaclePos(1680.f, 540.f);
	m_pTruck->SetColliderPos(0.f, 5.f);
	m_pTruck->SetColliderExtend(100.f, 60.f);

	// TrainSpawner Init
	m_pTrainSpawner_1->AddSpawnPosition(framework::Vector2D(741.f, -1300.f));
	m_pTrainSpawner_1->AddSpawnPosition(framework::Vector2D(741.f, ScreenHeight + 1300.f));
	m_pTrainSpawner_1->SetTriggerTime(static_cast<float>(1));
	m_pTrainSpawner_1->SetSpawnTime(static_cast<float>(spawnTimeDis(gen)));

	m_pTrainSpawner_2->AddSpawnPosition(framework::Vector2D(1061.f, -1300.f));
	m_pTrainSpawner_2->AddSpawnPosition(framework::Vector2D(1061.f, ScreenHeight + 1300.f));
	m_pTrainSpawner_2->SetTriggerTime(static_cast<float>(triggerTimeDis2(gen)));
	m_pTrainSpawner_2->SetSpawnTime(static_cast<float>(spawnTimeDis(gen)));

	m_pTrainSpawner_3->AddSpawnPosition(framework::Vector2D(1381.f, -1300.f));
	m_pTrainSpawner_3->AddSpawnPosition(framework::Vector2D(1381.f, ScreenHeight + 1300.f));
	m_pTrainSpawner_3->SetTriggerTime(static_cast<float>(triggerTimeDis3(gen)));
	m_pTrainSpawner_3->SetSpawnTime(static_cast<float>(spawnTimeDis(gen)));

	/// Player FSM Attach
	m_pPlayerFSM1 = new PlayerFSM1;
	m_pPlayerFSM2 = new PlayerFSM2;
	m_pPlayerFSM3 = new PlayerFSM3;
	m_pPlayerFSM4 = new PlayerFSM4;
	m_pPlayerObject1->AttachFSM(m_pPlayerFSM1);
	m_pPlayerObject2->AttachFSM(m_pPlayerFSM2);
	m_pPlayerObject3->AttachFSM(m_pPlayerFSM3);
	m_pPlayerObject4->AttachFSM(m_pPlayerFSM4);

	// SetGameObjectLocation
	m_pPlayerObject1->SetGameObjectLocation(m_Player1Point.x, m_Player1Point.y);
	m_pPlayerObject2->SetGameObjectLocation(m_Player2Point.x, m_Player2Point.y);
	m_pPlayerObject3->SetGameObjectLocation(m_Player3Point.x, m_Player3Point.y);
	m_pPlayerObject4->SetGameObjectLocation(m_Player4Point.x, m_Player4Point.y);

	// CollisionManager
	m_pCollisionManager->EnableCollisionType(GROUP_TYPE::PLAYER, GROUP_TYPE::PLAYER);			// 플레이어 - 플레이어
	m_pCollisionManager->EnableCollisionType(GROUP_TYPE::PLAYER, GROUP_TYPE::MONEY);			// 플레이어 - 돈
	m_pCollisionManager->EnableCollisionType(GROUP_TYPE::PLAYER, GROUP_TYPE::ITEMBOX);		// 플레이어 - 아이템 박스
	m_pCollisionManager->EnableCollisionType(GROUP_TYPE::PLAYER, GROUP_TYPE::ITEM);			// 플레이어 - 아이템
	m_pCollisionManager->EnableCollisionType(GROUP_TYPE::PLAYER, GROUP_TYPE::TRAIN);			// 플레이어 - 기차
	m_pCollisionManager->EnableCollisionType(GROUP_TYPE::PLAYER, GROUP_TYPE::OBSTACLE);		// 플레이어 - 장애물
	m_pCollisionManager->EnableCollisionType(GROUP_TYPE::PLAYER, GROUP_TYPE::Environment);	// 플레이어 - 환경
	m_pCollisionManager->EnableCollisionType(GROUP_TYPE::PLAYER, GROUP_TYPE::TRUCK);			// 플레이어 - 트럭

	m_pCollisionManager->EnableCollisionType(GROUP_TYPE::ITEM, GROUP_TYPE::OBSTACLE);			// 아이템 - 장애물
	m_pCollisionManager->EnableCollisionType(GROUP_TYPE::ITEM, GROUP_TYPE::TRAIN);			// 아이템 - 기차
	m_pCollisionManager->EnableCollisionType(GROUP_TYPE::ITEM, GROUP_TYPE::Environment);		// 아이템 - 환경
	m_pCollisionManager->EnableCollisionType(GROUP_TYPE::ITEM, GROUP_TYPE::TRUCK);			// 아이템 - 트럭

	m_pCollisionManager->EnableCollisionType(GROUP_TYPE::OBSTACLE, GROUP_TYPE::OBSTACLE);		// 장애물 - 장애물
	m_pCollisionManager->EnableCollisionType(GROUP_TYPE::OBSTACLE, GROUP_TYPE::TRAIN);		// 기차 - 장애물
	m_pCollisionManager->EnableCollisionType(GROUP_TYPE::OBSTACLE, GROUP_TYPE::Environment);	// 장애물 - 환경



	/// 사용할 아이템 설정
	m_ItemList[0].resize(static_cast<int>(eItemType::THROW_END) - static_cast<int>(eItemType::THROW_START) - 1);
	m_ItemList[1].resize(static_cast<int>(eItemType::INSTALLATION_END) - static_cast<int>(eItemType::INSTALLATION_START) - 1);
	m_ItemList[2].resize(static_cast<int>(eItemType::REINFORCED_END) - static_cast<int>(eItemType::REINFORCED_START) - 1);

	int assignItem = static_cast<int>(eItemType::THROW_START) + 1;
	for (size_t i = 0; i < m_ItemList[0].size(); i++)
	{
		m_ItemList[0][i] = static_cast<eItemType>(assignItem);
		++assignItem;
	}
	assignItem = static_cast<int>(eItemType::INSTALLATION_START) + 1;
	for (size_t i = 0; i < m_ItemList[1].size(); i++)
	{
		m_ItemList[1][i] = static_cast<eItemType>(assignItem);
		++assignItem;
	}
	assignItem = static_cast<int>(eItemType::REINFORCED_START) + 1;
	for (size_t i = 0; i < m_ItemList[2].size(); i++)
	{
		m_ItemList[2][i] = static_cast<eItemType>(assignItem);
		++assignItem;
	}

	return true;
}

void InGameWorld::Update(const float deltaTime)
{
	// 최고 스코어가 목표 금액의 절반을 넘는다면
	if (m_CurWinnerScore >= m_GoalScore / 2)
	{
		// 기차 스포너 활성화
		m_pTrainSpawner_1->SetIsRunning(true);
		m_pTrainSpawner_2->SetIsRunning(true);
		m_pTrainSpawner_3->SetIsRunning(true);
	}

	for(const auto& player : m_PlayerList)
	{
		if (m_CurWinnerScore < player->GetScore())
		{
			m_CurWinnerScore = player->GetScore();
			m_CurWinner = player->GetPlayerNumber();
		}
	}

	if(IsGameClear())
	{
		this->SetWorldPauseState(true);
		m_pInGameResultPopUp->SetActiveObj(true);
		m_pInGameResultPopUp->SetPopUpState(true);
		m_pInGameResultPopUp->SetWinner(m_CurWinner, m_PlayerList[m_CurWinner]->GetGameObjectLocation());
	}

	World::Update(deltaTime);
}

void InGameWorld::Finalize()
{
	if (m_pPlayerFSM1 && m_pPlayerFSM2 && m_pPlayerFSM3 && m_pPlayerFSM4)
	{
		delete m_pPlayerFSM1;
		m_pPlayerFSM1 = nullptr;
		delete m_pPlayerFSM2;
		m_pPlayerFSM2 = nullptr;
		delete m_pPlayerFSM3;
		m_pPlayerFSM3 = nullptr;
		delete m_pPlayerFSM4;
		m_pPlayerFSM4 = nullptr;
	}
	m_PlayerList.clear();
	World::Finalize();
}

void InGameWorld::Enter()
{
	std::cout << "InGameWorld Enter" << std::endl;

	// 이거
	m_CurWinnerScore = 0;

	EventManager::GetInstance()->RegisterListener(eEventType::DeleteGameObject, dynamic_cast<EventListener*>(this));
	
	SoundManager::GetInstance()->PlayMusic(eSoundList::Map1BGM, eSoundChannel::BGM, 0.6f);

	this->Initialize();
}

void InGameWorld::Exit()
{
	std::cout << "InGameWorld Exit" << std::endl;

	SoundManager::GetInstance()->StopMusic(eSoundChannel::BGM);

	this->Finalize();
}

eItemType InGameWorld::GetRandomItem(eItemBoxType itemBoxType)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> dis(1, 100);
	int randomNum = dis(gen);

	if (itemBoxType == eItemBoxType::THROW)
	{
		UINT startIdx = static_cast<UINT>(eItemType::THROW_START);
		// 펀치 30%
		if(randomNum >= 1 && randomNum <= 30)
			return m_ItemList[static_cast<UINT>(itemBoxType)][static_cast<UINT>(eItemType::PunchItem) - startIdx - 1];
		// 눈덩이 40%
		if (randomNum >= 31 && randomNum <= 70)
			return m_ItemList[static_cast<UINT>(itemBoxType)][static_cast<UINT>(eItemType::SnowItem) - startIdx - 1];
		// 전기충격기 10%
		if (randomNum >= 71 && randomNum <= 80)
			return m_ItemList[static_cast<UINT>(itemBoxType)][static_cast<UINT>(eItemType::StungunItem) - startIdx - 1];
		// 유리병 20%
		if (randomNum >= 81 && randomNum <= 100)
			return m_ItemList[static_cast<UINT>(itemBoxType)][static_cast<UINT>(eItemType::BottleItem) - startIdx - 1];
	}
	if (itemBoxType == eItemBoxType::INSTALLATION)
	{
		UINT startIdx = static_cast<UINT>(eItemType::INSTALLATION_START);
		// 덫 60%
		if (randomNum >= 1 && randomNum <= 60)
			return m_ItemList[static_cast<UINT>(itemBoxType)][static_cast<UINT>(eItemType::SnareItem) - startIdx - 1];
		// 함정 40%
		if (dis(gen) >= 61 && dis(gen) <= 100)
			return m_ItemList[static_cast<UINT>(itemBoxType)][static_cast<UINT>(eItemType::TrapItem) - startIdx - 1];
	}
	if (itemBoxType == eItemBoxType::REINFORCED)
	{
		UINT startIdx = static_cast<UINT>(eItemType::REINFORCED_START);
		// 신발 45%
		if (randomNum >= 1 && randomNum <= 45)
			return m_ItemList[static_cast<UINT>(itemBoxType)][static_cast<UINT>(eItemType::ShoeItem) - startIdx - 1];
		// 투명화 45%
		if (randomNum >= 46 && randomNum <= 90)
			return m_ItemList[static_cast<UINT>(itemBoxType)][static_cast<UINT>(eItemType::TransparencyItem) - startIdx - 1];
		// 해일 10%
		if (randomNum >= 91 && randomNum <= 100)
			return m_ItemList[static_cast<UINT>(itemBoxType)][static_cast<UINT>(eItemType::WaveItem) - startIdx - 1];
	}
	return eItemType::NOTHING;
}

void InGameWorld::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::DeleteGameObject)
	{
		DeleteGameObject(event->objGroup, event->obj);
	}
	else if (event->eventID == eEventType::CheckQuitGame)
	{
		this->SetWorldPauseState(true);
		m_pInGamePopUpUI1->SetActiveObj(true);
		m_pInGamePopUpUI1->SetPopUpState(true);
	}
	else if (event->eventID == eEventType::Continue)
	{
		this->SetWorldPauseState(false);
		m_pInGamePopUpUI1->SetActiveObj(false);
		m_pInGamePopUpUI1->SetPopUpState(false);
	}
	else if (event->eventID == eEventType::ChangeWorldToMain)
	{
		m_pInGamePopUpUI1->SetActiveObj(false);
		m_pInGamePopUpUI1->SetPopUpState(false);
		m_pInGamePopUpUI2->SetActiveObj(false);
		m_pInGamePopUpUI2->SetPopUpState(false);
		m_pInGameResultPopUp->SetActiveObj(false);
		m_pInGameResultPopUp->SetPopUpState(false);
		EventManager::GetInstance()->SendEvent(eEventType::PauseOff);
		m_pWorldManager->ChangeWorld(WORLD_TYPE::MAIN);
	}
	else if (event->eventID == eEventType::PauseOn)
	{
		this->SetWorldPauseState(true);
	}
	else if (event->eventID == eEventType::PauseOff)
	{
		this->SetWorldPauseState(false);
	}
}