#pragma once

#include "../Engine/FSMState.h"

#include "PlayerTransition1.h"

class PlayerIdleState1
	: public FSMState
{
private:
	IdleToMove1 m_IdleToMove;
	StateToStun1 m_IdleToStun;

public:
	PlayerIdleState1();

public:
	void SetDefaultAnimationName() override;

	void OnEnter() final;
	void OnStay() final;
	void OnExit() final;
};

