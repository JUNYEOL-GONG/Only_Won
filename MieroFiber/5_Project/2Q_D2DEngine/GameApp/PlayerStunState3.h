#pragma once
#include "../Engine/FSMState.h"

#include "PlayerTransition3.h"

class PlayerStunState3
	: public FSMState
{
private:
	StunToIdle3 m_StunToIdle;

public:
	PlayerStunState3();

public:
	void OnEnter() final;
	void OnStay() final;
	void OnExit() final;
};
