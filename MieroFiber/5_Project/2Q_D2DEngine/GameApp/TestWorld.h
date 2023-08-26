#pragma once

#include "../Engine/World.h"
#include "../Engine/EventListener.h"

#include <vector>

#include "ItemBoxObject.h"
#include "ItemObject.h"

class PlayerObject;
class PlayerFSM1;
class PlayerFSM2;
class PlayerFSM3;
class PlayerFSM4;
class MoneyObject;

class TestWorld
	: public World
	, public EventListener
{
private:
	PlayerObject* m_pPlayerObject1 = nullptr;
	PlayerObject* m_pPlayerObject2 = nullptr;
	PlayerObject* m_pPlayerObject3 = nullptr;
	PlayerObject* m_pPlayerObject4 = nullptr;
	PlayerFSM1* m_pPlayerFSM1 = nullptr;
	PlayerFSM2* m_pPlayerFSM2 = nullptr;
	PlayerFSM3* m_pPlayerFSM3 = nullptr;
	PlayerFSM4* m_pPlayerFSM4 = nullptr;

	MoneyObject* m_pMoneyObject = nullptr;
	ItemBoxObject* m_pItemBoxObject1 = nullptr;
	ItemBoxObject* m_pItemBoxObject2 = nullptr;

	std::vector<eItemType> m_ItemList[static_cast<UINT>(eItemBoxType::END_ITEMBOX)];

public:
	TestWorld();
	~TestWorld() final;

public:
	eItemType GetRandomItem(eItemBoxType itemType);

public:
	bool Initialize() final;
	void Update(const float deltaTime) final;
	void Finalize() final;

	void Enter() final;
	void Exit() final;

public:
	void HandleEvent(Event* event) final;
};