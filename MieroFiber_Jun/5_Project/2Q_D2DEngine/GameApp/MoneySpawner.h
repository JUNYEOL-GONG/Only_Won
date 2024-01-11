#pragma once

#include "MoneyObject.h"
#include "Spawner.h"
#include "../Engine/EventListener.h"

class Event;
enum eMoneyType;

class MoneyObject;

class MoneySpawner
	: public Spawner
	, public EventListener
{
private:
    MoneyObject* m_pSpawnObject = nullptr;

	eMoneyType m_moneyType = SPAWNED;

	int m_curCount = 0;

	float m_elapsedTime = 0;

public:
	bool Initialize() final;
	void Update(const float deltaTime) final;

public:
	void SetType(eMoneyType moneyType);
	void RandomSpawn();

public:
	void HandleEvent(Event* event) final;
};

