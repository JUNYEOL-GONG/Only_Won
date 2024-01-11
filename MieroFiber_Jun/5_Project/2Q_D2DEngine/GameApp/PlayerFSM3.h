#pragma once
#include "../Engine/FSM.h"

#include "PlayerIdleState3.h"
#include "PlayerMoveState3.h"
#include "PlayerStunState3.h"

class PlayerFSM3
	: public FSM
{
private:
	PlayerIdleState3 m_IdleState;
	PlayerMoveState3 m_MoveState;
	PlayerStunState3 m_StunState;

public:
	PlayerFSM3();
};
