#pragma once
#include "../Engine/FSMState.h"

#include "PlayerTransition1.h"

class PlayerStunState1
	: public FSMState
{
private:
	StunToIdle1 m_StunToIdle;

public:
	PlayerStunState1();

public:
	void OnEnter() final;
	void OnStay() final;
	void OnExit() final;
};

