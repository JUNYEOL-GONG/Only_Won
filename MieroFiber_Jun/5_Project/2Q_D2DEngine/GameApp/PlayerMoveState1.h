#pragma once
#include "../Engine/FSMState.h"

#include "PlayerTransition1.h"

class PlayerMoveState1
	: public FSMState
{
private:
	MoveToIdle1 m_MoveToIdle;
	StateToStun1 m_MoveToStun;

public:
	PlayerMoveState1();

public:
	void OnEnter() final;
	void OnStay() final;
	void OnExit() final;
};

