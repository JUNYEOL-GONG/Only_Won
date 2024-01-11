#pragma once

#include "../Engine/GameObject.h"

class BoxComponent;
class AnimationComponent;
class PlayerMovement;
class FSMComponent;
class FSM;
class EventListener;
class DirectionComponent;

class PlayerObject0
	: public GameObject
{
private:
	BoxComponent* m_pBoxComponent = nullptr;
	AnimationComponent* m_pAnimComponent = nullptr;
	PlayerMovement* m_pMovementComponent = nullptr;
	FSMComponent* m_pFSMComponent = nullptr;
	FSM* m_pPlayerFSM = nullptr;
	DirectionComponent* m_pDirectionComponent = nullptr;


public:
	~PlayerObject0() override;

public:
	bool Initialize() override;
	void Update(const float deltaTime) override;
};