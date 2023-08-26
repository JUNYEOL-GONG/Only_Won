#include "framework.h"
#include "ItemSpawner.h"

#include "ItemBoxObject.h"

#include <random>

#include "../Engine/EventManager.h"
#include "../Engine/World.h"


bool ItemSpawner::Initialize()
{
	EventManager::GetInstance()->RegisterListener(eEventType::DeleteThrow, this);
	EventManager::GetInstance()->RegisterListener(eEventType::DeleteInstallation, this);
	EventManager::GetInstance()->RegisterListener(eEventType::DeleteReinforced, this);

	while (m_curCount < m_maxCount)
	{
		m_gameObjectList = m_pOwnerWorld->GetGameObjectGroup(GROUP_TYPE::ITEMBOX);
		RandomSpawn();
	}

	__super::Initialize();


	return true;
}

void ItemSpawner::Update(const float deltaTime)
{
	__super::Update(deltaTime);
	m_gameObjectList = m_pOwnerWorld->GetGameObjectGroup(GROUP_TYPE::ITEMBOX);

	// 설정해둔 최대갯수에 도달하면 리턴
	if (m_curCount == m_maxCount)
		return;

	if (!m_bCheckAABB)
	{
		for (size_t i = 0; i < m_elapsedTimeList.size(); i++)
		{
			if(m_elapsedTimeList[i] != -1.f)
				m_elapsedTimeList[i] += deltaTime;
		}
	}

	for (size_t i = 0; i < m_elapsedTimeList.size(); i++)
	{
		if(m_elapsedTimeList[i] >= m_spawnTime)
		{
			m_elapsedTimeList[i] = -1.f;
			RandomSpawn();
		}
	}

	__super::Update(deltaTime);
}

void ItemSpawner::SetType(eItemBoxType itemType)
{
	m_itemBoxType = itemType;
}

void ItemSpawner::RandomSpawn()
{
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

	if (m_gameObjectList.empty())
	{
		if (m_itemBoxType == eItemBoxType::THROW)
		{
			m_pSpawnObject =
				m_pOwnerWorld->CreateGameObject<ItemBoxObject>(L"Throw_Item", GROUP_TYPE::ITEMBOX);
		}
		else if (m_itemBoxType == eItemBoxType::REINFORCED)
		{
			m_pSpawnObject =
				m_pOwnerWorld->CreateGameObject<ItemBoxObject>(L"Reinforced_Item", GROUP_TYPE::ITEMBOX);
		}
		else if (m_itemBoxType == eItemBoxType::INSTALLATION)
		{
			m_pSpawnObject =
				m_pOwnerWorld->CreateGameObject<ItemBoxObject>(L"Installation_Item", GROUP_TYPE::ITEMBOX);
		}
		m_pSpawnObject->Initialize();
		m_pSpawnObject->SetGameObjectLocation(m_randX, m_randY);
		m_pSpawnObject->SetItemBoxType(m_itemBoxType);

		m_curCount++;
		return;
	}

	float x = 0.f;
	float y = 0.f;

	for(size_t i = 0; i < m_gameObjectList.size(); i++)
	{
		x =
			dynamic_cast<ItemBoxObject*>(m_gameObjectList[i])->GetGameObjectLocation().x;
		y =
			dynamic_cast<ItemBoxObject*>(m_gameObjectList[i])->GetGameObjectLocation().y;

		if (CheckSpawnArea(x, y))
		{
			m_bCheckAABB = true;
			break;
		}
		m_bCheckAABB = false;
	}

	if (m_bCheckAABB)
	{
		RandomSpawn();
	}
	else
	{
		if (m_itemBoxType == eItemBoxType::THROW)
		{
			m_pSpawnObject =
				m_pOwnerWorld->CreateGameObject<ItemBoxObject>(L"Throw_Item", GROUP_TYPE::ITEMBOX);
		}
		else if (m_itemBoxType == eItemBoxType::REINFORCED)
		{
			m_pSpawnObject =
				m_pOwnerWorld->CreateGameObject<ItemBoxObject>(L"Reinforced_Item", GROUP_TYPE::ITEMBOX);
		}
		else if (m_itemBoxType == eItemBoxType::INSTALLATION)
		{
			m_pSpawnObject =
				m_pOwnerWorld->CreateGameObject<ItemBoxObject>(L"Installation_Item", GROUP_TYPE::ITEMBOX);
		}
		m_pSpawnObject->SetItemBoxType(m_itemBoxType);
		m_pSpawnObject->Initialize();
		m_pSpawnObject->SetGameObjectLocation(m_randX, m_randY);
		m_bCheckAABB = false;
		m_curCount++;
	}
}

void ItemSpawner::HandleEvent(Event* event)
{
	if(event->eventID == eEventType::DeleteThrow && m_itemBoxType == eItemBoxType::THROW)
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
	else if (event->eventID == eEventType::DeleteInstallation && m_itemBoxType == eItemBoxType::INSTALLATION)
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
	else if (event->eventID == eEventType::DeleteReinforced && m_itemBoxType == eItemBoxType::REINFORCED)
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