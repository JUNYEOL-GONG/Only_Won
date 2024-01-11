#pragma once
#include "ItemObject.h"

class PlayerObejct;

class ShoeItem
	: public ItemObject
{
private:
	PlayerObject* m_pPlayer = nullptr;

	static float m_shoeElapsedTime;

public:
	bool Initialize() final;
	void Update(const float deltaTime) final;
};

