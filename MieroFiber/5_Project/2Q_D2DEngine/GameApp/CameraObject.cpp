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

// todo(기림) : 카메라 트랜스폼 Invert 외에 LateUpdate 필요할 시 메인루프 쪽으로 빼는 작업 필요함
void CameraObject::LateUpdate()
{
	m_pCameraComponent->InvertCameraTransform();
}