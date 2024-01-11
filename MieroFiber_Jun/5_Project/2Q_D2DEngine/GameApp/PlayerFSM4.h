#pragma once

#include "../Engine/FSM.h"

#include "PlayerIdleState4.h"
#include "PlayerMoveState4.h"
#include "PlayerStunState4.h"

class PlayerFSM4
	: public FSM
{
private:
	PlayerIdleState4 m_IdleState;
	PlayerMoveState4 m_MoveState;
	PlayerStunState4 m_StunState;

public:
	PlayerFSM4();
};

