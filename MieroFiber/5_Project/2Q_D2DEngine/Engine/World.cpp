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
	// �Ͻ� ���� ���¿��� UI�� ������Ʈ �ǰ� �ϱ�
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

		// �̰� �ۿ��ٰ� �ѱ� else �ȿ��ٰ� �ѱ� -> UI�� Ȱ��ȭ �Ǿ����� ���� �ݸ��� �ʿ�����ϱ�.. else �ȿ��ٰ� �Ӵϴ�.
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
	/// GroupType ������� ������ (���̾��� ����)

	// Only Won�� ��� ī�޶� �����̱� ������ ������ ����ȭ�� �ʿ� ����
	// AABB �ִ� ������ 230809 ���� Ŀ�� �� �������� ����
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
	// ���� �ٲ� ������ ���� �ִ� ������Ʈ���� �������ش�
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
				break; // for�� ��ü�� Ż��
		}
	}
}

//void World::Save(const wchar_t* FilePath)
//{
//	nlohmann::ordered_json obj;
//}