#include "pch.h"
#include "PlayerMovement.h"

#include "GameObject.h"
#include "InputManager.h"
#include "SceneComponent.h"
#include "EventManager.h"

void PlayerMovement::KeyDirControl()
{
	// MoveDir�� LookDir�� Ű���� �Է°����� �����Ѵ�

	const float xInupt = InputManager::GetInstance()->GetAxisRaw("Horizontal");
	const float yInupt = InputManager::GetInstance()->GetAxisRaw("Vertical");

	m_MoveDir = { xInupt, yInupt };
	m_MoveDir.Normalize();

	// ������ LookDir�� �����ϴ� ���??

	const framework::Vector2D vec = { xInupt, yInupt };

	// �Է��� �������� LookDir�� �����Ѵ�
	if(vec.Length() >= 0.1f)
		m_LookDir = { xInupt, yInupt };

	m_LookDir.Normalize();
}

void PlayerMovement::TotalDirControl(int controllerIndex)
{
	MoveDirControl(controllerIndex);
	LookDirControl(controllerIndex);
}

void PlayerMovement::MoveDirControl(int controllerIndex)
{
	// ���� �̵� ������ ���
	m_PrevMoveDir = m_MoveDir;

	// �̵� ���� �ʱ�ȭ
	m_MoveDir = { 0.f, 0.f };

	// ���� / ������ ��ƽ �Է°�
	const framework::Vector2D rawThumbLeft = InputManager::GetPadAxisLeftThumb(controllerIndex);
	const framework::Vector2D rawThumbRight = InputManager::GetPadAxisRightThumb(controllerIndex);

	// ������ ���� ��ƽ �Է°�
	framework::Vector2D thumbLeft = { max(-1, rawThumbLeft.x), max(-1, (-1) * rawThumbLeft.y) };

	// �Է°��� ���� �̵� ���� ����
	m_MoveDir = { thumbLeft.x, thumbLeft.y };

	// �̵� ������ ���� ���͸� ���Ѵ�
	m_MoveDir.Normalize();

	// ������ ��ƽ�� �Է°��� ���� ��� �̵� ������ �ٶ󺸴� �������� �����Ѵ�
	if (rawThumbRight.x == 0.f || rawThumbRight.y == 0.f)
	{
		// �̵� ������ ���� ��쿡�� �ٶ󺸴� �������� �����Ѵ� (�ڵ����� �̵����� ������ �ٶ󺸴� ������ ���� �ٶ󺸴� �������� ������)
		if (m_MoveDir != framework::Vector2D::Zero())
			m_LookDir = m_MoveDir;
	}
}

void PlayerMovement::LookDirControl(int controllerIndex)
{
	// ������ ��ƽ �Է°�
	const framework::Vector2D rawThumbRight = InputManager::GetPadAxisRightThumb(controllerIndex);

	// ������ ������ ��ƽ�� �Է°�
	framework::Vector2D thumbRight = { max(-1, rawThumbRight.x), max(-1, (-1) * rawThumbRight.y) };

	// ������ ��ƽ�� �Է°��� �ִ� ��쿡�� ������ �ٲ��ش�
	if (thumbRight.x != 0.f || thumbRight.y != 0.f)
		m_LookDir = { thumbRight.x, thumbRight.y };

	// �ٶ󺸴� ������ ���� ���͸� ���Ѵ�
	m_LookDir.Normalize();
}

void PlayerMovement::PlayerMove(float deltaTime)
{
	/// �Է� ������ �ٲ�� ���� �ݴ�� �� �� �ֵ���
	if (std::signbit(m_PrevMoveDir.x * m_MoveDir.x))
		m_OriginalVelocity.x = 0.f;
	if (std::signbit(m_PrevMoveDir.y * m_MoveDir.y))
		m_OriginalVelocity.y = 0.f;

	/// ���ӵ� (�ӵ��� ��ȭ��) ���
	m_OriginalAccel =  m_MoveDir * m_OriginalAccelSpeed * m_ReverseCoefficient;			// �Է°��� ���� �������� ���ӵ�
	m_TotalAccel = m_OriginalAccel + m_ExternalAccel;									// �������� ���ӵ� + �ܺ� ���ӵ� = ���� ���ӵ�

	/// �ӵ� (��ġ�� ��ȭ��) ���
	m_OriginalVelocity += m_TotalAccel * deltaTime;										// �ӵ� = �ӵ� + ���ӵ� * �ð�

	// m_OriginalVelocity Limit
	m_OriginalVelocity.Limit(m_OriginalVelocityLimit);									// �������� �ӵ� ����

	// m_ExternalVelocity Limit
	m_ExternalVelocity.Limit(m_ExternalVelocityLimit);									// �ܺ� �ӵ� ����

	m_TotalVelocity = m_OriginalVelocity + m_ExternalVelocity;							// �������� �ӵ� + �ܺ� �ӵ� = ���� �ӵ�

	// �̵� �Է� ������ ������ �ۿ�
	if (m_MoveDir == framework::Vector2D::Zero())
		OriginalFriction(deltaTime);

	// �ܺ� �ӵ��� ���� �������� �׽� �۵�
	ExternalFriction(deltaTime);

	// ��ȭ�Ǵ� �ӵ��� �÷��̾ �̵�
	m_pOwner->GetRootComponent()->AddRelativeLocation(m_TotalVelocity * deltaTime);
}

void PlayerMovement::OriginalFriction(float deltaTime)
{
	// OriginalFriction
	if (m_OriginalVelocity.Length() <= 50.f)
		m_OriginalVelocity = framework::Vector2D::Zero();
	else
	{
		const framework::Vector2D friction1 = m_OriginalVelocity.GetNormalize() * (-1) * m_OriginalAccelSpeed;
		m_OriginalVelocity += friction1 * deltaTime;
	}
}

void PlayerMovement::ExternalFriction(float deltaTime)
{
	// ExternalFriction
	if (m_ExternalVelocity.Length() <= 50.f)
		m_ExternalVelocity = framework::Vector2D::Zero();
	else
	{
		const framework::Vector2D friction2 = m_ExternalVelocity.GetNormalize() * (-1) * m_ExternalAccelSpeed;
		m_ExternalVelocity += friction2 * deltaTime;
	}
}

void PlayerMovement::KnockBack(framework::Vector2D dir, float power)
{
	m_ExternalVelocity = dir * power;
}

void PlayerMovement::ReverseMove()
{
	if (!m_IsReverseMove)
	{
		m_ReverseCoefficient = -1.f;
		m_IsReverseMove = true;
	}
	else
	{
		m_ReverseCoefficient = 1.f;
		m_IsReverseMove = false;
	}
}

void PlayerMovement::Update(const float deltaTime)
{
	// Ȱ��ȭ ���� �ƴϸ� �ƹ��͵� ���� �ʴ´�
	if (!m_IsActive)
		return;

	// 1P ~ 4P������ ��Ʈ�ѷ� �Է��� �޾ƿ´�
	if (m_pOwner->GetName() == L"Player1")
	{
		TotalDirControl(0);

		// ��Ʈ�ѷ��� ����Ǿ� ������, ���� ��ƽ�� ������ ��ƽ ��� �Է��� ���� ��쿡 Ű����� ����
		if((InputManager::GetPadAxisLeftThumb(0).Length() < 0.001f) && (InputManager::GetPadAxisRightThumb(0).Length() < 0.001f))
		{
			KeyDirControl();
		}
	}
	else if (m_pOwner->GetName() == L"Player2")
		TotalDirControl(1);
	else if (m_pOwner->GetName() == L"Player3")
		TotalDirControl(2);
	else if (m_pOwner->GetName() == L"Player4")
		TotalDirControl(3);

	PlayerMove(deltaTime);

	/// Transition�� ���� ����
	const framework::EVENT_MOVEMENT_INFO movementInfo = { m_MoveDir, m_LookDir };

	if (m_pOwner->GetName() == L"Player1")
		EventManager::GetInstance()->SendEvent(eEventType::P1TransperMovement, movementInfo);
	else if (m_pOwner->GetName() == L"Player2")
		EventManager::GetInstance()->SendEvent(eEventType::P2TransperMovement, movementInfo);
	else if (m_pOwner->GetName() == L"Player3")
		EventManager::GetInstance()->SendEvent(eEventType::P3TransperMovement, movementInfo);
	else if (m_pOwner->GetName() == L"Player4")
		EventManager::GetInstance()->SendEvent(eEventType::P4TransperMovement, movementInfo);
}