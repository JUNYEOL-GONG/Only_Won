#pragma once
#include "../Engine/FSMState.h"

#include "PlayerTransition3.h"

class PlayerMoveState3
	: public FSMState
{
private:
	MoveToIdle3 m_MoveToIdle;
	StateToStun3 m_MoveToStun;

public:
	PlayerMoveState3();

public:
	void OnEnter() final;
	void OnStay() final;
	void OnExit() final;
};

