#pragma once

#include "../Engine/World.h"
#include "../Engine/EventListener.h"

class PlayerObject;
class PlayerFSM1;
class PlayerFSM2;
class BoxObject;
class TrainSpawner;

class JunWorld
	: public World
	, public EventListener
{
private:
	PlayerObject* m_pPlayer_1 = nullptr;
	PlayerObject* m_pPlayer_2 = nullptr;

	PlayerFSM1* m_pPlayerFSM1 = nullptr;
	PlayerFSM2* m_pPlayerFSM2 = nullptr;

	BoxObject* m_pWall_1 = nullptr;
	BoxObject* m_pWall_2 = nullptr;
	BoxObject* m_pWall_3 = nullptr;
	BoxObject* m_pWall_4 = nullptr;

	TrainSpawner* m_pTrainSpawner_1 = nullptr;
	TrainSpawner* m_pTrainSpawner_2 = nullptr;
	TrainSpawner* m_pTrainSpawner_3 = nullptr;

public:
	JunWorld();
	~JunWorld() final;

	bool Initialize() final;
	void Update(const float deltaTime) final;
	void Finalize() final;

	void Enter() final;
	void Exit() final;

public:
	void HandleEvent(Event* event) final;
};