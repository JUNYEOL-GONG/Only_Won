#pragma once

#include "../Engine/FSMState.h"

#include "PlayerTransition2.h"

class PlayerMoveState2
	: public FSMState
{
private:
	MoveToIdle2 m_MoveToIdle;
	StateToStun2 m_MoveToStun;

public:
	PlayerMoveState2();

public:
	void OnEnter() final;
	void OnStay() final;
	void OnExit() final;
};

