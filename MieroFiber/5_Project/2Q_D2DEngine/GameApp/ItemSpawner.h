#pragma once
#include "ItemBoxObject.h"
#include "Spawner.h"
#include "../Engine/EventListener.h"

#include <list>

enum class eItemBoxType;

class ItemBoxObject;

class ItemSpawner
	: public Spawner
	, public EventListener
{
	int m_curCount;

	bool m_checkOnce = false;

	ItemBoxObject* m_pSpawnObject = nullptr;

	eItemBoxType m_itemBoxType = eItemBoxType::THROW;

public:
	bool Initialize() final;
	void Update(const float deltaTime) final;

public:
	void SetType(eItemBoxType itemType);
	void RandomSpawn();

	void HandleEvent(Event* event) final;
};