#include "pch.h"
#include "CameraComponent.h"

D2D_MATRIX_3X2_F CameraComponent::m_CameraTransform = D2D1::Matrix3x2F::Identity();

bool CameraComponent::Initialize()
{
	SceneComponent::Initialize();
	return true;
}

void CameraComponent::Update(const float deltaTime)
{
	m_CameraTransform = m_WorldTransform * D2D1::Matrix3x2F::Translation(-ScreenWidth / 2.f, -ScreenHeight / 2.f);

	__super::Update(deltaTime);
}