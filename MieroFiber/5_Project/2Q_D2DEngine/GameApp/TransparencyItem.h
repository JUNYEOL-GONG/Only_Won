#pragma once
#include "ItemObject.h"

class PlayerObject;
class PlayerMovement;

class TransparencyItem
	: public ItemObject
{
private:
	PlayerObject* m_pPlayer = nullptr;
	PlayerMovement* m_pPlayerMovement = nullptr;

	framework::Vector2D m_curPlayerLocation = {};
	UINT m_playerNum;

	static float transElapsedTime;
	static float transfootprintElapsedTime;

	bool m_bIsRunning = false;

public:
	bool Initialize() final;
	void Update(const float deltaTime) final;
};
