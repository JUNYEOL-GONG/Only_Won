#pragma once

#include "../Engine/FSMState.h"

#include "PlayerTransition4.h"

class PlayerIdleState4
	: public FSMState
{
private:
	IdleToMove4 m_IdleToMove;
	StateToStun4 m_IdleToStun;

public:
	PlayerIdleState4();

public:
	void SetDefaultAnimationName() override;

	void OnEnter() final;
	void OnStay() final;
	void OnExit() final;
};

