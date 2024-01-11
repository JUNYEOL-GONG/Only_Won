#include "framework.h"
#include "TestWorld.h"

#include "ItemBoxObject.h"
#include "MoneyObject.h"
#include "PlayerObject.h"
#include "PlayerFSM1.h"
#include "PlayerFSM2.h"

#include "../Engine/CollisionManager.h"

#include <random>

#include "../Engine/Event.h"
#include "../Engine/EventManager.h"

TestWorld::TestWorld()
{
	SetWorldName(L"TestWorld");
}

TestWorld::~TestWorld()
{
	if (m_pPlayerFSM1 && m_pPlayerFSM2)
	{
		delete m_pPlayerFSM1;
		delete m_pPlayerFSM2;
	}
}

eItemType TestWorld::GetRandomItem(eItemBoxType itemType)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	if(itemType == eItemBoxType::THROW)
	{
		std::uniform_int_distribution<> dis(0, static_cast<int>(m_ItemList[0].size() - 1));
		return m_ItemList[static_cast<UINT>(itemType)][dis(gen)];
	}
	if (itemType == eItemBoxType::INSTALLATION)
	{
		std::uniform_int_distribution<> dis(0, static_cast<int>(m_ItemList[1].size() - 1));
		return m_ItemList[static_cast<UINT>(itemType)][dis(gen)];
	}
	if (itemType == eItemBoxType::REINFORCED)
	{
		std::uniform_int_distribution<> dis(0, static_cast<int>(m_ItemList[2].size() - 1));
		return m_ItemList[static_cast<UINT>(itemType)][dis(gen)];
	}
	return eItemType::NOTHING;
}

bool TestWorld::Initialize()
{
	m_pPlayerObject1 = CreateGameObject<PlayerObject>(L"Player1", GROUP_TYPE::PLAYER);
	m_pPlayerObject2 = CreateGameObject<PlayerObject>(L"Player2", GROUP_TYPE::PLAYER);

	m_pMoneyObject = CreateGameObject<MoneyObject>(L"Money", GROUP_TYPE::MONEY);
	m_pItemBoxObject1 = CreateGameObject<ItemBoxObject>(L"ItemBox", GROUP_TYPE::ITEMBOX);
	m_pItemBoxObject2 = CreateGameObject<ItemBoxObject>(L"ItemBox", GROUP_TYPE::ITEMBOX);
	m_pItemBoxObject1->SetItemBoxType(eItemBoxType::THROW);
	m_pItemBoxObject2->SetItemBoxType(eItemBoxType::THROW);


	bool res = __super::Initialize();
	assert(res);

	/// Player FSM Attach
	m_pPlayerFSM1 = new PlayerFSM1;
	m_pPlayerFSM2 = new PlayerFSM2;
	//m_pPlayerFSM3 = new PlayerFSM3;
	//m_pPlayerFSM4 = new PlayerFSM4;
	m_pPlayerObject1->AttachFSM(m_pPlayerFSM1);
	m_pPlayerObject2->AttachFSM(m_pPlayerFSM2);

	/// SetGameObjectLocation
	m_pPlayerObject1->SetGameObjectLocation(ScreenWidth / 2.f - 300.f, ScreenHeight / 2.f);
	m_pPlayerObject2->SetGameObjectLocation(ScreenWidth / 2.f + 300.f, ScreenHeight / 2.f);

	m_pMoneyObject->SetGameObjectLocation(300.f, 300.f);
	m_pItemBoxObject1->SetGameObjectLocation(ScreenWidth / 2.f, ScreenHeight / 2.f);
	m_pItemBoxObject2->SetGameObjectLocation(ScreenWidth / 2.f + 100.f, ScreenHeight / 2.f);

	m_pCollisionManager->EnableCollisionType(GROUP_TYPE::PLAYER, GROUP_TYPE::PLAYER);
	m_pCollisionManager->EnableCollisionType(GROUP_TYPE::PLAYER, GROUP_TYPE::MONEY);
	m_pCollisionManager->EnableCollisionType(GROUP_TYPE::PLAYER, GROUP_TYPE::ITEMBOX);

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

void TestWorld::Update(const float deltaTime)
{
	static float testTime = 0.f;
	testTime += deltaTime;

	if(testTime>=3.f)
	{
		m_pMoneyObject->SetMoneyType(eMoneyType::OWNED);
	}

	World::Update(deltaTime);
}

void TestWorld::Finalize()
{
	if(m_pPlayerFSM1 && m_pPlayerFSM2)
	{
		delete m_pPlayerFSM1;
		delete m_pPlayerFSM2;
		m_pPlayerFSM1 = nullptr;
		m_pPlayerFSM2 = nullptr;
	}

	World::Finalize();
}

void TestWorld::Enter()
{
	std::cout << "TestWorld Enter" << std::endl;

	// DeleteObject 이벤트 처리할 월드 Listener 등록
	EventManager::GetInstance()->RegisterListener(eEventType::DeleteGameObject, dynamic_cast<EventListener*>(this));

	this->Initialize();
}

void TestWorld::Exit()
{
	std::cout << "TestWorld Exit" << std::endl;

	this->Finalize();
}

void TestWorld::HandleEvent(Event* event)
{
	if(event->eventID == eEventType::DeleteGameObject)
	{
		DeleteGameObject(event->objGroup, event->obj);
	}
}