#pragma once
#include "PlayerObject.h"

class PlayerFSM1;

class Player1
	: public PlayerObject
{
private:
	PlayerFSM1* m_pPlayerFSM = nullptr;

public:
	~Player1() final;

public:
	bool Initialize() final;

public:
	void HandleEvent(Event* event) final;
};

