#include "pch.h"
#include "SceneComponent.h"

#include "GameObject.h"

bool SceneComponent::Initialize()
{
	UpdateTransform();

	return true;
}

void SceneComponent::Update(float deltaTime)
{
	UpdateTransform();
}

void SceneComponent::UpdateTransform()
{
	// 복합 변환 - SRT
	if (m_RelativeRotation >= 360.f)	m_RelativeRotation -= 360.f;

	m_RelativeTransform = D2D1::Matrix3x2F::Scale(m_RelativeScale.x, m_RelativeScale.y)
		* D2D1::Matrix3x2F::Rotation(m_RelativeRotation)
		* D2D1::Matrix3x2F::Translation(m_RelativeLocation.x, m_RelativeLocation.y);

	// 부모 컴포넌트를 가지고	있는 경우 (자식 컴포넌트인 경우)
	if (m_pParentComponent)
	{
		// worldTransform 계산
		m_WorldTransform = m_RelativeTransform * m_pParentComponent->m_WorldTransform;
		m_WorldLocation = { m_WorldTransform.dx, m_WorldTransform.dy };
	}
	// 부모 컴포넌트인 경우
	else
	{
		if (m_pParentObject)
		{
			m_WorldTransform = m_RelativeTransform * m_pParentObject->GetRootComponent()->m_WorldTransform;
		}
		else
		{
			m_WorldTransform = m_RelativeTransform;
		}
		m_WorldLocation = { m_WorldTransform.dx, m_WorldTransform.dy };

	}
}

void SceneComponent::AttachToComponent(SceneComponent* pParent)
{
	m_pParentComponent = pParent;
	pParent->m_Children.push_back(this);
}