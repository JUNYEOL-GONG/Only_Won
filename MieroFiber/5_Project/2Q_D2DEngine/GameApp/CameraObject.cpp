#include "framework.h"
#include "CameraObject.h"

#include "../Engine/CameraComponent.h"

bool CameraObject::Initialize()
{
	m_pCameraComponent = CreateComponent<CameraComponent>(L"CameraComponent");
	m_pCameraComponent->SetRelativeLocation(0.f, 0.f);
	SetRootComponent(m_pCameraComponent);

	bool res = __super::Initialize();
	assert(res);

	return true;
}

void CameraObject::Update(const float deltaTime)
{
	__super::Update(deltaTime);

	LateUpdate();
}

// todo(�⸲) : ī�޶� Ʈ������ Invert �ܿ� LateUpdate �ʿ��� �� ���η��� ������ ���� �۾� �ʿ���
void CameraObject::LateUpdate()
{
	m_pCameraComponent->InvertCameraTransform();
}