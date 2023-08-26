#pragma once
#include "../Engine/FSMState.h"

#include "PlayerTransition2.h"

class PlayerStunState2
	: public FSMState
{
private:
	StunToIdle2 m_StunToIdle;

public:
	PlayerStunState2();

public:
	void OnEnter() final;
	void OnStay() final;
	void OnExit() final;
};

