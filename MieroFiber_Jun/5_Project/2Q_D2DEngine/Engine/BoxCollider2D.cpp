#include "pch.h"
#include "BoxCollider2D.h"

#include "CommonApp.h"
#include "GameObject.h"

#include "CircleCollider2D.h"

bool BoxCollider2D::Initialize()
{
	const bool res = SceneComponent::Initialize();
	assert(res);

	return true;
}

void BoxCollider2D::Update(const float deltaTime)
{
	Collider2D::Update(deltaTime);
	m_collider.center = SceneComponent::GetWorldLocation();
}

void BoxCollider2D::Render(ID2D1RenderTarget* pRenderTarget)
{
	m_FinalTransform = m_WorldTransform * GetCameraTransform();
	pRenderTarget->SetTransform(m_FinalTransform);

	// 충돌 여부에 따른 콜라이더 박스 변화
	if (m_IsCollision)
	{
		SetColor(D2D1::ColorF::Red);
		CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);

		pRenderTarget->DrawRectangle(
			D2D1::RectF(-m_collider.extend.x, -m_collider.extend.y, m_collider.extend.x, m_collider.extend.y),
			CommonApp::m_pInstance->GetBrush(),
			2.f
		);
	}
	else if (m_IsTrigger)
	{
		SetColor(D2D1::ColorF::Yellow);
		CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);

		pRenderTarget->DrawRectangle(
			D2D1::RectF(-m_collider.extend.x, -m_collider.extend.y, m_collider.extend.x, m_collider.extend.y),
			CommonApp::m_pInstance->GetBrush(),
			2.f
		);
	}
	else
	{
		SetColor(D2D1::ColorF::Green);
		CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);

		if (m_name == L"BoxCollider2D_Head")
		{
			SetColor(D2D1::ColorF::Cyan);
			CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);
		}

		pRenderTarget->DrawRectangle(
			D2D1::RectF(-m_collider.extend.x, -m_collider.extend.y, m_collider.extend.x, m_collider.extend.y),
			CommonApp::m_pInstance->GetBrush(),
			2.f
		);
	}

	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

bool BoxCollider2D::CheckIntersect(Collider2D* otherCollision) const
{
	// other가 BoxCollider2D인 경우
	const BoxCollider2D* temp1 = dynamic_cast<BoxCollider2D*>(otherCollision);
	if (temp1)
		return m_collider.Check_AABB_AABB(dynamic_cast<BoxCollider2D*>(otherCollision)->m_collider);

	// other가 CircleCollider2D인 경우
	const CircleCollider2D* temp2 = dynamic_cast<CircleCollider2D*>(otherCollision);
	if (temp2)
		return m_collider.Check_AABB_CC(dynamic_cast<CircleCollider2D*>(otherCollision)->m_collider);

	return false;
}

void BoxCollider2D::SetExtend(float x, float y)
{
	// 중심점에서 확장되는 길이
	m_collider.extend.x = x;
	m_collider.extend.y = y;
}

void BoxCollider2D::OnCollisionEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{
	// std::wcout << this->m_pOwner->GetName() << L" <-----> " << otherCollision->GetOwner()->GetName() << L" OnCollisionEnter" << std::endl;

	m_pOwner->OnCollisionEnter(thisCollision, otherCollision, iter);
}

void BoxCollider2D::OnCollisionStay(Collider2D* otherCollision)
{
	// std::wcout << this->m_pOwner->GetName() << L" " << otherCollision->GetOwner()->GetName() << L" OnCollisionStay" << std::endl;

	m_pOwner->OnCollisionStay(otherCollision);
}

void BoxCollider2D::OnCollisionExit(Collider2D* otherCollision)
{
	// std::wcout << this->m_pOwner->GetName() << L" <-----> " << otherCollision->GetOwner()->GetName() << L" OnCollisionExit" << std::endl;

	m_pOwner->OnCollisionExit(otherCollision);
}

void BoxCollider2D::OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{
	// std::wcout << this->m_pOwner->GetName() << L" <-----> " << otherCollision->GetOwner()->GetName() << L" OnTriggerEnter" << std::endl;

	m_pOwner->OnTriggerEnter(thisCollision, otherCollision, iter);
}

void BoxCollider2D::OnTriggerStay(Collider2D* otherCollision)
{
	// std::wcout << this->m_pOwner->GetName() << L" " << otherCollision->GetOwner()->GetName() << L" OnTriggerStay" << std::endl;

	m_pOwner->OnTriggerStay(otherCollision);
}

void BoxCollider2D::OnTriggerExit(Collider2D* otherCollision)
{
	// std::wcout << this->m_pOwner->GetName() << L" <-----> " << otherCollision->GetOwner()->GetName() << L" OnTriggerExit" << std::endl;

	m_pOwner->OnTriggerExit(otherCollision);
}

void BoxCollider2D::ProcessBlock(Collider2D* otherCollision)
{
	// 1. 두 오브젝트의 extendSum 구하기
	const float extendSumX = this->GetExtend().x + dynamic_cast<BoxCollider2D*>(otherCollision)->GetExtend().x;
	const float extendSumY = this->GetExtend().y + dynamic_cast<BoxCollider2D*>(otherCollision)->GetExtend().y;

	const GameObject* thisOwner = this->GetOwner();
	const GameObject* otherOwner = dynamic_cast<BoxCollider2D*>(otherCollision)->GetOwner();

	// 2. 두 오브젝트의 중심점 구하기
	const framework::Vector2D thisObjectPos = thisOwner->GetRootComponent()->GetWorldLocation();
	const framework::Vector2D otherObjectPos = otherOwner->GetRootComponent()->GetWorldLocation();

	// 3. 두 오브젝트의 콜라이더의 중심점
	const framework::Vector2D thisColliderCenter = this->GetCenter();
	const framework::Vector2D otherColliderCenter = dynamic_cast<BoxCollider2D*>(otherCollision)->GetCenter();

	// 두 오브젝트 사이의 거리 구하기
	const float distX = abs(thisColliderCenter.x - otherColliderCenter.x);
	const float distY = abs(thisColliderCenter.y - otherColliderCenter.y);

	// 밀어낼 값을 계산 (겹쳐진 길이 == 밀어낼 값)
	float pushLengthX = extendSumX - distX;
	float pushLengthY = extendSumY - distY;

	// 오 - 왼 위치일 때 : pushX, pushY 음수로
	// X축 밀기
	if (pushLengthX <= pushLengthY)
	{
		if (thisObjectPos.x <= otherObjectPos.x)
			pushLengthX *= -1.f;

		thisOwner->GetRootComponent()->AddRelativeLocation(pushLengthX, 0.f);
	}
	// Y축 밀기
	else
	{
		if (thisObjectPos.y <= otherObjectPos.y)
			pushLengthY *= -1.f;

		thisOwner->GetRootComponent()->AddRelativeLocation(0.f, pushLengthY);
	}
}