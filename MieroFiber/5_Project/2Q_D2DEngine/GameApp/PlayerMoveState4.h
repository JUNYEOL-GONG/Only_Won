#include "../Engine/FSMState.h"

#include "PlayerTransition4.h"

class PlayerMoveState4
	: public FSMState
{
private:
	MoveToIdle4 m_MoveToIdle;
	StateToStun4 m_MoveToStun;

public:
	PlayerMoveState4();

public:
	void OnEnter() final;
	void OnStay() final;
	void OnExit() final;
};

