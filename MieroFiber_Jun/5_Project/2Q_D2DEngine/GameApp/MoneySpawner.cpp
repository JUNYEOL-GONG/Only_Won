#include "framework.h"
#include "MoneySpawner.h"

#include "MoneyObject.h"
#include "PlayerObject.h"

#include "../Engine/World.h"
#include "../Engine/EventManager.h"

#include <random>

#include "InGameWorld.h"

class Event;

bool MoneySpawner::Initialize()
{
	EventManager::GetInstance()->RegisterListener(eEventType::DeleteMoney, this);

	while(m_curCount < m_maxCount)
	{
		m_gameObjectList = m_pOwnerWorld->GetGameObjectGroup(GROUP_TYPE::MONEY);
		RandomSpawn();
	}
	__super::Initialize();

	return true;
}

void MoneySpawner::Update(const float deltaTime)
{
	std::vector<PlayerObject*>& playerList =
		dynamic_cast<InGameWorld*>(this->GetOwnerWorld())->GetPlayerList();
	std::vector<GameObject*>& moneyVec = m_pOwnerWorld->GetGameObjectGroup(GROUP_TYPE::MONEY);

	int countDropMoney = 0;
	int countSpawnMoney = 0;
	int countPlayerMoney = 0;

	for(size_t i = 0; i < moneyVec.size(); i++)
	{
		if (dynamic_cast<MoneyObject*>(moneyVec[i])->GetMoneyType() == eMoneyType::DROPED_UNCOLLISION
			|| dynamic_cast<MoneyObject*>(moneyVec[i])->GetMoneyType() == eMoneyType::DROPED_COLLISION)
			countDropMoney++;
		if (dynamic_cast<MoneyObject*>(moneyVec[i])->GetMoneyType() == eMoneyType::SPAWNED)
			countSpawnMoney++;
	}

	for(size_t i = 0; i < playerList.size(); i++)
	{
		countPlayerMoney += playerList[i]->GetMoney();
	}

	int totalMoney = countDropMoney + countSpawnMoney + countPlayerMoney;

	if (totalMoney == m_maxCount)
	{
		return;
	}

	if (totalMoney == 0)
	{
		while (m_curCount < m_maxCount)
		{
			m_gameObjectList = m_pOwnerWorld->GetGameObjectGroup(GROUP_TYPE::MONEY);
			RandomSpawn();
		}
		for(size_t i = 0; i < m_elapsedTimeList.size(); i++)
		{
			m_elapsedTimeList[i] = -1;
		}
		return;
	}

	if (!m_bCheckAABB)
	{
		for (size_t i = 0; i < m_maxCount - totalMoney; i++)
		{
			if (m_elapsedTimeList[i] != -1.f)
				m_elapsedTimeList[i] += deltaTime;
		}
	}

	for (size_t i = 0; i < m_elapsedTimeList.size(); i++)
	{
		if (m_elapsedTimeList[i] >= m_spawnTime)
		{
			m_elapsedTimeList[i] = -1.f;
			RandomSpawn();
		}
	}
	__super::Update(deltaTime);
}

void MoneySpawner::SetType(eMoneyType moneyType)
{
	m_moneyType = moneyType;
}

void MoneySpawner::RandomSpawn()
{
	static int cnt = 0;
	static int recallCount = 0;

	// 재귀함수가 15번 불리면
	if (recallCount >= 15)
	{
		return;
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, static_cast<UINT>(m_spawnArea.size()) - 1);

	// 랜덤으로 구한 영역
	size_t curSpawnAreaIndex;
	if (m_spawnArea.size() > 1)
		curSpawnAreaIndex = dis(gen);
	else
		curSpawnAreaIndex = 0;
	m_curSpawnArea = m_spawnArea[curSpawnAreaIndex];

	// 영역 랜덤 구하기
	std::random_device rd1;
	std::mt19937 gen1(rd1());
	std::uniform_int_distribution<> dis1(static_cast<int>(m_curSpawnArea.left), static_cast<int>(m_curSpawnArea.right));
	m_randX = static_cast<float>(dis1(gen1));

	std::random_device rd2;
	std::mt19937 gen2(rd2());
	std::uniform_int_distribution<> dis2(static_cast<int>(m_curSpawnArea.top), static_cast<int>(m_curSpawnArea.bottom));
	m_randY = static_cast<float>(dis2(gen2));

	if(m_gameObjectList.empty())
	{
		MoneyObject* spawnedMoney =
			m_pOwnerWorld->CreateGameObject<MoneyObject>(L"Money", GROUP_TYPE::MONEY);
		//m_pSpawnObject =
		spawnedMoney->Initialize();
		spawnedMoney->SetGameObjectLocation(m_randX, m_randY);
		spawnedMoney->SetMoneyType(m_moneyType);

		m_curCount++;
		return;
	}

	m_gameObjectList = m_pOwnerWorld->GetGameObjectGroup(GROUP_TYPE::MONEY);

	for (size_t i = 0; i < m_gameObjectList.size(); i++)
	{
		// eMoneyType이 spawn 외에 다른거랑은 판별해줄 필요 없음.
		if (dynamic_cast<MoneyObject*>(m_gameObjectList[i])->GetMoneyType()
			!= eMoneyType::SPAWNED)
			continue;
		const float x =
			m_gameObjectList[i]->GetGameObjectLocation().x;
		const float y =
			m_gameObjectList[i]->GetGameObjectLocation().y;

		if (CheckSpawnArea(x, y))
		{
			m_bCheckAABB = true;
			break;
		}
		m_bCheckAABB = false;
	}

	if(m_bCheckAABB)
	{
		recallCount++;
		RandomSpawn();
		recallCount = 0;
	}
	else
	{
		MoneyObject* spawnedMoney =
			m_pOwnerWorld->CreateGameObject<MoneyObject>(L"Money", GROUP_TYPE::MONEY);
		//m_pSpawnObject =
		spawnedMoney->Initialize();
		spawnedMoney->SetGameObjectLocation(m_randX, m_randY);
		spawnedMoney->SetMoneyType(m_moneyType);
		m_bCheckAABB = false;
		m_curCount++;
		recallCount = 0;
	}
}

void MoneySpawner::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::DeleteMoney && m_moneyType == eMoneyType::SPAWNED)
	{
		for (size_t i = 0; i < m_elapsedTimeList.size(); i++)
		{
			if (m_elapsedTimeList[i] == -1.f)
			{
				m_curCount--;
				m_elapsedTimeList[i] = 0;
				break;
			}
		}
	}
}
