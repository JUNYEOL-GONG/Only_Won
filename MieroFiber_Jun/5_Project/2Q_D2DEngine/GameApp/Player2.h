#pragma once

#include "PlayerObject.h"

class PlayerFSM2;

class Player2
	: public PlayerObject
{
private:
	PlayerFSM2* m_pPlayerFSM = nullptr;

public:
	~Player2() final;

public:
	bool Initialize() final;

public:
	void HandleEvent(Event* event) final;
};

