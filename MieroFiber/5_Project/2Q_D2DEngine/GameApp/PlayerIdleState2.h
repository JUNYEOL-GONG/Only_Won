#pragma once

#include "../Engine/FSMState.h"

#include "PlayerTransition2.h"

class PlayerIdleState2
	: public FSMState
{
private:
	IdleToMove2 m_IdleToMove;
	StateToStun2 m_IdleToStun;

public:
	PlayerIdleState2();

public:
	void SetDefaultAnimationName() override;

	void OnEnter() final;
	void OnStay() final;
	void OnExit() final;
};

