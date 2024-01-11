#include "pch.h"
#include "World.h"
//#include "nlohmann/json.hpp"

#include "CollisionManager.h"
#include "InputManager.h"
#include "../GameApp/Bar1Object.h"
#include "../GameApp/PlayerObject.h"

#include "../GameApp/UIObject.h"
#include "../GameApp/PopUpUIObject.h"

World::World()
{
	m_pCollisionManager = new CollisionManager;
}

World::~World()
{
	delete m_pCollisionManager;

	World::Finalize();
}

bool World::Initialize()
{
	for (auto& gameObjectGroup : m_GameObjects)
	{
		for (const auto& gameObject : gameObjectGroup)
		{
			const bool res = gameObject->Initialize();
			assert(res);
		}
	}

	m_pCollisionManager->Initialize();

	return true;
}

void World::Update(const float deltaTime)
{
	// 일시 정지 상태에는 UI만 업데이트 되게 하기
	if(m_bPauseState)
	{
		for(auto& gameObjectGroup : m_GameObjects[static_cast<UINT>(GROUP_TYPE::UI)])
		{
			gameObjectGroup->Update(deltaTime);
		}
	}

	else
	{
		// Object Update
		for (auto& gameObjectGroup : m_GameObjects)
		{
			for (const auto& gameObject : gameObjectGroup)
				if (gameObject->IsObjActive())
					gameObject->Update(deltaTime);
		}

		// 이걸 밖에다가 둘까 else 안에다가 둘까 -> UI만 활성화 되어있을 때는 콜리전 필요없으니까.. else 안에다가 둡니다.
		// Object Collision Update
		m_pCollisionManager->Update(deltaTime, m_GameObjects);
	}

	if (m_GameObjects[static_cast<UINT>(GROUP_TYPE::BARUI)].empty())
		return;

	for (size_t i = 0; i < m_GameObjects[static_cast<UINT>(GROUP_TYPE::BARUI)].size()-1; i++)
	{
		for (size_t j = i + 1; j < m_GameObjects[static_cast<UINT>(GROUP_TYPE::BARUI)].size(); j++)
		{
			if(dynamic_cast<PlayerObject*>(m_GameObjects[static_cast<UINT>(GROUP_TYPE::BARUI)][i]->GetOwnerObj())->GetScore()
				< dynamic_cast<PlayerObject*>(m_GameObjects[static_cast<UINT>(GROUP_TYPE::BARUI)][j]->GetOwnerObj())->GetScore())
			{
				std::swap(m_GameObjects[static_cast<UINT>(GROUP_TYPE::BARUI)][i],
					m_GameObjects[static_cast<UINT>(GROUP_TYPE::BARUI)][j]);
			}
		}
	}
}

void World::Render()
{
	/// GroupType 순서대로 렌더링 (레이어의 개념)

	// Only Won의 경우 카메라가 고정이기 때문에 렌더링 최적화가 필요 없음
	// AABB 있는 버전은 230809 오후 커밋 전 리비전에 있음
	for (auto& gameObjectGroup : m_GameObjects)
	{
		for (const auto& gameObject : gameObjectGroup)
		{
			if(gameObject->IsObjActive())
				gameObject->Render();
		}
	}
}

void World::Finalize()
{
	// 씬이 바뀔 때마다 씬에 있는 오브젝트들을 삭제해준다
	for (auto& gameObjectGroup : m_GameObjects)
	{
		for (const auto& gameObject : gameObjectGroup)
			delete gameObject;

		gameObjectGroup.clear();
	}
}

void World::DeleteGameObject(GROUP_TYPE group, const GameObject* gameObject)
{
	for(auto iter = m_GameObjects[static_cast<UINT>(group)].begin(); iter!= m_GameObjects[static_cast<UINT>(group)].end(); ++iter)
	{
		if(*iter == gameObject)
		{
			delete *iter;
			iter = m_GameObjects[static_cast<UINT>(group)].erase(iter);

			if(iter == m_GameObjects[static_cast<UINT>(group)].end())
				break; // for문 자체를 탈출
		}
	}
}

//void World::Save(const wchar_t* FilePath)
//{
//	nlohmann::ordered_json obj;
//}