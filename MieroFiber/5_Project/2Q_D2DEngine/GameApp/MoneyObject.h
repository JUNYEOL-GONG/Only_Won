#pragma once
#include "../Engine/GameObject.h"

class TextureComponent;
class BoxCollider2D;

enum eMoneyType
{
	SPAWNED,
	OWNED,
	DROPED_UNCOLLISION,
	DROPED_COLLISION,
};

class MoneyObject
	: public GameObject
{
private:
	eMoneyType m_MoneyType = eMoneyType::SPAWNED;
	TextureComponent* m_pTextureComponent = nullptr;
	BoxCollider2D* m_pCollider = nullptr;

	float m_ElapsedTime = 0.f;
	float m_DelayTime = 0.1f;

public:
	void SetMoneyType(eMoneyType moneyType) { m_MoneyType = moneyType; }
	eMoneyType GetMoneyType() { return m_MoneyType; }

public:
	bool Initialize() final;
	void Update(const float deltaTime) final;
};

