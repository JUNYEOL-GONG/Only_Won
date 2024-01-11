#pragma once
#include "../Engine/GameObject.h"

class TextureComponent;
class BarUI;

class Bar3Object
	: public GameObject
{
private:
	BarUI* m_pBarUI = nullptr;
	TextureComponent* m_pBarTexture = nullptr;
	TextureComponent* m_pBarFlagTexture = nullptr;

	float m_scaleX = -1.f;
	float m_scaleY = 1.f;

	float m_relativeX = -550.f;

	int m_prevMoney = 0;

	int m_goalMoney = 0;
	// µ· ÇÑ¹¶Ä¡
	int m_moneyBundle = 1;
	// ºñÀ²
	float m_barRate = 0.f;


public:
	bool Initialize() final;
	void Update(const float deltaTime) final;
};

