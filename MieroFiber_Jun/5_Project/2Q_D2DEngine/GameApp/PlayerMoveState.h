#pragma once

#include "../Engine/FSMState.h"

#include "PlayerTransition.h"

class PlayerMoveState
	: public FSMState
{
private:
	MoveToIdle m_MoveToIdle;

public:
	PlayerMoveState();

public:
	void OnEnter() final;
	void OnStay() final;
	void OnExit() final;
};

