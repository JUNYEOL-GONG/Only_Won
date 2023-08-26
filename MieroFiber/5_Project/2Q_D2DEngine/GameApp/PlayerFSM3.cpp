#include "framework.h"
#include "PlayerFSM3.h"

PlayerFSM3::PlayerFSM3()
{
	AddState(L"IDLE", &m_IdleState);
	AddState(L"MOVE", &m_MoveState);
	AddState(L"STUN", &m_StunState);

	SetDefaultState(&m_IdleState);
}