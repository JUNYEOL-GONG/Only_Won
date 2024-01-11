#pragma once

#include "../Engine/GameObject.h"

class BoxComponent;
class BoxCollider2D;

class BoxObject :
    public GameObject
{
private:
	BoxComponent* m_pBoxComponent = nullptr;
	BoxCollider2D* m_pBoxCollider2D = nullptr;

public:
	bool Initialize() final;
	void Update(const float deltaTime) final;
};