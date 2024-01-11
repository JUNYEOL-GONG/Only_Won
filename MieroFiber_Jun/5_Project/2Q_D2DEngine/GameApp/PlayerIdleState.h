#pragma once

#include "../Engine/FSMState.h"
#include "PlayerTransition.h"

class PlayerIdleState
	: public FSMState
{
private:
	IdleToMove m_IdleToMove;

public:
	PlayerIdleState();

public:
	void OnEnter() final;
	void OnStay() final;
	void OnExit() final;
};

