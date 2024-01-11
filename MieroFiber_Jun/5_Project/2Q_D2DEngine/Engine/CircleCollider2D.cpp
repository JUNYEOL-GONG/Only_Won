#include "pch.h"
#include "CircleCollider2D.h"

#include "CommonApp.h"

#include "BoxCollider2D.h"

bool CircleCollider2D::Initialize()
{
	const bool res = SceneComponent::Initialize();
	assert(res);

	return true;
}

void CircleCollider2D::Update(const float deltaTime)
{
	Collider2D::Update(deltaTime);

	m_collider.center = SceneComponent::GetWorldLocation();
}

void CircleCollider2D::Render(ID2D1RenderTarget* pRenderTarget)
{
	m_FinalTransform = m_WorldTransform * GetCameraTransform();
	pRenderTarget->SetTransform(m_FinalTransform);

	// 충돌 여부에 따른 콜라이더 색상 변화
	if (m_IsCollision)
	{
		SetColor(D2D1::ColorF::Red);
		CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);

		pRenderTarget->DrawEllipse(
			D2D1::Ellipse(D2D1::Point2F(0.f, 0.f), m_collider.radius, m_collider.radius),
			CommonApp::m_pInstance->GetBrush(),
			5.0f
		);
	}
	else
	{
		SetColor(D2D1::ColorF::Green);
		CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);

		pRenderTarget->DrawEllipse(
			D2D1::Ellipse(D2D1::Point2F(0.f, 0.f), m_collider.radius, m_collider.radius),
			CommonApp::m_pInstance->GetBrush(),
			5.0f
		);
	}

	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

bool CircleCollider2D::CheckIntersect(Collider2D* other) const
{
	// other가 CircleCollider2D인 경우
	const CircleCollider2D* temp1 = dynamic_cast<CircleCollider2D*>(other);
	if (temp1)
		return m_collider.Check_CC_CC(dynamic_cast<CircleCollider2D*>(other)->m_collider);

	// other가 BoxCollider2D인 경우
	const BoxCollider2D* temp2 = dynamic_cast<BoxCollider2D*>(other);
	if (temp2)
		return m_collider.Check_CC_AABB(dynamic_cast<BoxCollider2D*>(other)->m_collider);

	return false;
}

void CircleCollider2D::SetExtend(float r)
{
	// 중심에서 확장되는 반지름 길이
	m_collider.radius = r;
}