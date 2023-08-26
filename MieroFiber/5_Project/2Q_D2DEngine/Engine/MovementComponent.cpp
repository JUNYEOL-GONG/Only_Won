#include "pch.h"
#include "MovementComponent.h"

#include <cmath>

#include "GameObject.h"
#include "InputManager.h"
#include "SceneComponent.h"
#include "EventManager.h"

inline int ConvertToRaw(const float& value)
{
	if (value > 0) return 1;
	else if (value < 0) return -1;
	else return 0;
}

void MovementComponent::MoveControl(int controllerIndex)
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

void MovementComponent::DirectionControl(int controllerIndex)
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

void MovementComponent::ApplyFriction()
{
	// ���� ��� (0 ~ 1 ������ ��)
	// constexpr float frictionCoefficient = 0.995f; // ������ ����
	constexpr float frictionCoefficient = 0.95f; // ������ ���� (��ӵ� ó��)

	// ������ ����
	m_Velocity.x *= frictionCoefficient;
	m_Velocity.y *= frictionCoefficient;
}

void MovementComponent::AddForce(framework::Vector2D vec)
{
	m_Velocity += vec;
}

/// <summary>
/// ��Ʈ�ѷ� ��� �׽�Ʈ
/// </summary>
/// <param name="controllerIndex"></param>
void MovementComponent::XInputTestFunc(int controllerIndex)
{
	/*
	/// A B X Y Input Test
	if (InputManager::GetInstance()->IsPadButtonPush(controllerIndex, GamePadButtonCode::A))
		std::cout << std::to_string(controllerIndex + 1) << "P�� A ��ư PUSH" << std::endl;
	else if (InputManager::GetInstance()->IsPadButtonEnd(controllerIndex, GamePadButtonCode::A))
		std::cout << std::to_string(controllerIndex + 1) << "P�� A ��ư END" << std::endl;

	if (InputManager::GetInstance()->IsPadButtonPush(controllerIndex, GamePadButtonCode::B))
		std::cout << std::to_string(controllerIndex + 1) << "P�� B ��ư PUSH" << std::endl;
	else if (InputManager::GetInstance()->IsPadButtonEnd(controllerIndex, GamePadButtonCode::B))
		std::cout << std::to_string(controllerIndex + 1) << "P�� B ��ư END" << std::endl;

	/// L1 R1 Input Test
	if (InputManager::GetInstance()->IsPadButtonPush(controllerIndex, GamePadButtonCode::L1))
		std::cout << std::to_string(controllerIndex + 1) << "P�� L1 ��ư PUSH" << std::endl;
	else if (InputManager::GetInstance()->IsPadButtonEnd(controllerIndex, GamePadButtonCode::L1))
	{
		std::cout << std::to_string(controllerIndex + 1) << "P�� L1 ��ư END" << std::endl;
		InputManager::SetControllerVib(controllerIndex, 0, 0);
	}

	if (InputManager::GetInstance()->IsPadButtonPush(controllerIndex, GamePadButtonCode::R1))
		std::cout << std::to_string(controllerIndex + 1) << "P�� R1 ��ư PUSH" << std::endl;
	else if (InputManager::GetInstance()->IsPadButtonEnd(controllerIndex, GamePadButtonCode::R1))
	{
		std::cout << std::to_string(controllerIndex + 1) << "P�� R1 ��ư END" << std::endl;
		InputManager::SetControllerVib(controllerIndex, 0, 0);
	}

	/// L3 R3 Input Test
	if (InputManager::GetInstance()->IsPadButtonPush(controllerIndex, GamePadButtonCode::L3))
		std::cout << std::to_string(controllerIndex + 1) << "P�� L3 PUSH" << std::endl;
	else if (InputManager::GetInstance()->IsPadButtonEnd(controllerIndex, GamePadButtonCode::L3))
		std::cout << std::to_string(controllerIndex + 1) << "P�� L3 END" << std::endl;

	if (InputManager::GetInstance()->IsPadButtonPush(controllerIndex, GamePadButtonCode::R3))
		std::cout << std::to_string(controllerIndex + 1) << "P�� R3 PUSH" << std::endl;
	else if (InputManager::GetInstance()->IsPadButtonEnd(controllerIndex, GamePadButtonCode::R3))
		std::cout << std::to_string(controllerIndex + 1) << "P�� R3 END" << std::endl;

	/// START BACK Input Test
	if (InputManager::GetInstance()->IsPadButtonPush(controllerIndex, GamePadButtonCode::START))
		std::cout << std::to_string(controllerIndex + 1) << "P�� START PUSH" << std::endl;
	else if (InputManager::GetInstance()->IsPadButtonEnd(controllerIndex, GamePadButtonCode::START))
		std::cout << std::to_string(controllerIndex + 1) << "P�� START END" << std::endl;

	if (InputManager::GetInstance()->IsPadButtonPush(controllerIndex, GamePadButtonCode::BACK))
		std::cout << std::to_string(controllerIndex + 1) << "P�� BACK PUSH" << std::endl;
	else if (InputManager::GetInstance()->IsPadButtonEnd(controllerIndex, GamePadButtonCode::BACK))
		std::cout << std::to_string(controllerIndex + 1) << "P�� BACK END" << std::endl;

	/// L2 R2 Trigger Input Test
	const float triggerL = InputManager::GetPadAxisLeftTrigger(controllerIndex);
	const float triggerR = InputManager::GetPadAxisRightTrigger(controllerIndex);

	if (triggerL > 0.2f || triggerR > 0.2f)
	{
		InputManager::SetControllerVib(controllerIndex, static_cast<WORD>(triggerL * 65535),
			static_cast<WORD>(triggerR * 65535));
	}
	*/


	/*-----------------------------------------------------------------------------------
	-------------------------------------------------------------------------------------

				���⼭���� Movement ���� �Լ� �ڵ��̰� ���� ������ Input Test Code

	-------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------*/

	MoveControl(controllerIndex);
	DirectionControl(controllerIndex);
}

void MovementComponent::Update(const float deltaTime)
{
	// 1P ~ 4P������ ��Ʈ�ѷ� �Է��� �޾ƿ´�
	if (m_pOwner->GetName() == L"Player1")
		XInputTestFunc(0);
	else if (m_pOwner->GetName() == L"Player2")
		XInputTestFunc(1);
	else if (m_pOwner->GetName() == L"Player3")
		XInputTestFunc(2);
	else if (m_pOwner->GetName() == L"Player4")
		XInputTestFunc(3);


	/// �Է� ������ �ٲ�� ���� �ݴ�� �� �� �ֵ���
	if (std::signbit(m_PrevMoveDir.x * m_MoveDir.x))
		m_Velocity.x = 0.f;
	if (std::signbit(m_PrevMoveDir.y * m_MoveDir.y))
		m_Velocity.y = 0.f;

	/// 1�� �ĺ� (�̰ɷ� �ϴ°� �´µ�,,,?)
	m_Velocity.x += m_MoveDir.x * m_AccelSpeed * deltaTime;
	m_Velocity.y += m_MoveDir.y * m_AccelSpeed * deltaTime;

	/// Velocity Limit
	m_Velocity.Limit(300.f);

	if (m_pOwner->GetName() == L"Player1")
	{
		if (InputManager::GetInstance()->IsPadButtonPush(0, GamePadButtonCode::R3))
		{
			const framework::Vector2D temp = { 3000.f, 3000.f };
			AddForce(temp);
		}
	}

	/// �̵� �Է� ������ ������ �ۿ�
	if (m_MoveDir == framework::Vector2D::Zero())
		ApplyFriction();

	// ���� �ӵ��� ���� �÷��̾� �̵�
	m_pOwner->GetRootComponent()->AddRelativeLocation(m_Velocity * deltaTime);

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