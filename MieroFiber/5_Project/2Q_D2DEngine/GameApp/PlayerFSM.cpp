#include "framework.h"
#include "PlayerFSM.h"

PlayerFSM::PlayerFSM()
{
	AddState(L"IDLE", &m_IdleState);
	AddState(L"MOVE", &m_MoveState);
	SetDefaultState(&m_IdleState);
}
