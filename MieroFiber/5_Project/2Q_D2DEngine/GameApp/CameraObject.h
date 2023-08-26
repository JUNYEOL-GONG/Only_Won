#pragma once

#include "../Engine/GameObject.h"

class CameraComponent;

class CameraObject
	: public GameObject
{
private:
	CameraComponent* m_pCameraComponent = nullptr;

public:
	virtual bool Initialize() override final;
	virtual void Update(const float deltaTime) override final;

private:
	void LateUpdate();
};

