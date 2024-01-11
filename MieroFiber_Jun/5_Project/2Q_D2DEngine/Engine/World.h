#pragma once

#include "GameObject.h"

#include <utility>
#include <vector>
#include <cassert>
#include <list>

enum class WORLD_TYPE
{
	TEST = 0,
	JUN,
	CHAE,
	MAIN,
	INSTRUCTION,
	MADEBY,
	GAMESETTING,
	INGAME,


	END
};

class UIObject;
class PopUpUIObject;
class CollisionManager;
class WorldManager;

/// <summary>
/// World�� ���� Ŭ����
/// GameApp�� World ���� ������ ���带 �˻�, ��ȯ�Ѵ�.
/// World�� GameObject ����Ʈ�� ��� �ִ�.
/// </summary>
class World
{
protected:
	UINT m_ID = 0;
	std::wstring m_WorldName = {};

	// ���尡 �Ͻ� ���� �Ǿ�����
	bool m_bPauseState = false;

private:
	std::vector<GameObject*> m_GameObjects[static_cast<UINT>(GROUP_TYPE::END)];
	std::vector<GameObject*> m_RenderGameObjects;

protected:
	CollisionManager* m_pCollisionManager = nullptr;
	WorldManager* m_pWorldManager = nullptr;

public:
	World();
	virtual ~World();

public:
	virtual bool Initialize();
	virtual void Update(const float deltaTime);
	virtual void Render();
	virtual void Finalize();

	virtual void Enter() abstract;
	virtual void Exit() abstract;

public:
	void Save(const wchar_t* FilePath);

	void SetWorldManager(WorldManager* worldManager) { m_pWorldManager = worldManager; }

	void SetWorldPauseState(bool pauseState) { m_bPauseState = pauseState; }
	bool GetWorldPauseState() { return m_bPauseState; }

	//std::list<GameObject*>& GetGameObjectGroup(GROUP_TYPE type)	{return m_GameObjects[static_cast<UINT>(type)];}
	std::vector<GameObject*>& GetGameObjectGroup(GROUP_TYPE type)	{return m_GameObjects[static_cast<UINT>(type)];}

	UINT GetID() const { return m_ID; }
	void SetID(UINT id) { m_ID = id; }

	std::wstring GetWorldName() const { return m_WorldName; }
	void SetWorldName(std::wstring worldName) { m_WorldName = std::move(worldName); }

protected:
	void DeleteGameObject(GROUP_TYPE group, const GameObject* gameObject);

public:
	template <typename T>
	T* CreateGameObject(std::wstring objectName, GROUP_TYPE objectType)
	{
		T* obj = new T;

		GameObject* castedObj = dynamic_cast<GameObject*>(obj);
		assert(castedObj);
		castedObj->SetName(objectName);									// �ڽ��� �̸� ����
		castedObj->SetOwnerWorld(this);										// �ڽ��� ���� ���� ����

		castedObj->SetGroupType(objectType);								// �ڽ��� �׷� ����
		m_GameObjects[static_cast<UINT>(objectType)].push_back(castedObj);	// ���忡 �ڽ��� ���

		return obj;
	}
};