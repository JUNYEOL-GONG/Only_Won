#pragma once
#include "../Engine/FSMState.h"

#include "PlayerTransition4.h"

class PlayerStunState4
	: public FSMState
{
private:
	StunToIdle4 m_StunToIdle;

public:
	PlayerStunState4();

public:
	void OnEnter() final;
	void OnStay() final;
	void OnExit() final;
};
