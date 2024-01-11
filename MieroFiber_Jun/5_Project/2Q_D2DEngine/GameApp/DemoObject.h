#pragma once

#include "../Engine/GameObject.h"

class BoxComponent;

class DemoObject
	: public GameObject
{
private:
	BoxComponent* m_pBoxComponent1 = nullptr;
	BoxComponent* m_pBoxComponent2 = nullptr;
	BoxComponent* m_pBoxComponent3 = nullptr;

public:
	bool Initialize() final;
	void Update(const float deltaTime) final;
};