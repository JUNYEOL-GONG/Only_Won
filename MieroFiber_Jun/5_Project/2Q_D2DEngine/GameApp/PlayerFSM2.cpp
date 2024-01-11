#include "framework.h"
#include "PlayerFSM2.h"

PlayerFSM2::PlayerFSM2()
{
	AddState(L"IDLE", &m_IdleState);
	AddState(L"MOVE", &m_MoveState);
	AddState(L"STUN", &m_StunState);

	SetDefaultState(&m_IdleState);
}