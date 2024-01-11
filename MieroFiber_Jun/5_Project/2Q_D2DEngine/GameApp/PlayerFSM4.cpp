#include "framework.h"
#include "PlayerFSM4.h"

PlayerFSM4::PlayerFSM4()
{
	AddState(L"IDLE", &m_IdleState);
	AddState(L"MOVE", &m_MoveState);
	AddState(L"STUN", &m_StunState);

	SetDefaultState(&m_IdleState);
}