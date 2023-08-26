#pragma once
#include "../Engine/FSMState.h"

#include "PlayerTransition3.h"

class PlayerIdleState3
	: public FSMState
{
private:
	IdleToMove3 m_IdleToMove;
	StateToStun3 m_IdleToStun;

public:
	PlayerIdleState3();

public:
	void SetDefaultAnimationName() override;

	void OnEnter() final;
	void OnStay() final;
	void OnExit() final;
};

