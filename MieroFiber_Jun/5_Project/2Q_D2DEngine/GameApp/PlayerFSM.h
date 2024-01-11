#pragma once

#include "../Engine/FSM.h"
#include "PlayerIdleState.h"
#include "PlayerMoveState.h"

class PlayerFSM
	: public FSM
{
private:
	PlayerIdleState m_IdleState;
	PlayerMoveState m_MoveState;

public:
	PlayerFSM();
};

