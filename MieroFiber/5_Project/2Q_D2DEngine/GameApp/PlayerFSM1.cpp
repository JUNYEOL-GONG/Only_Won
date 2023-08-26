#include "framework.h"
#include "PlayerFSM1.h"

PlayerFSM1::PlayerFSM1()
{
	AddState(L"IDLE", &m_IdleState);
	AddState(L"MOVE", &m_MoveState);
	AddState(L"STUN", &m_StunState);

	SetDefaultState(&m_IdleState);
}