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
	// 이전 이동 방향을 기록
	m_PrevMoveDir = m_MoveDir;

	// 이동 방향 초기화
	m_MoveDir = { 0.f, 0.f };

	// 왼쪽 / 오른쪽 스틱 입력값
	const framework::Vector2D rawThumbLeft = InputManager::GetPadAxisLeftThumb(controllerIndex);
	const framework::Vector2D rawThumbRight = InputManager::GetPadAxisRightThumb(controllerIndex);

	// 보정된 왼쪽 스틱 입력값
	framework::Vector2D thumbLeft = { max(-1, rawThumbLeft.x), max(-1, (-1) * rawThumbLeft.y) };

	// 입력값에 따른 이동 방향 설정
	m_MoveDir = { thumbLeft.x, thumbLeft.y };

	// 이동 방향의 단위 벡터를 구한다
	m_MoveDir.Normalize();

	// 오른쪽 스틱의 입력값이 없는 경우 이동 방향을 바라보는 방향으로 설정한다
	if (rawThumbRight.x == 0.f || rawThumbRight.y == 0.f)
	{
		// 이동 방향이 없는 경우에만 바라보는 방향으로 설정한다 (자동으로 이동방향 없으면 바라보는 방향은 이전 바라보는 방향으로 설정됨)
		if (m_MoveDir != framework::Vector2D::Zero())
			m_LookDir = m_MoveDir;
	}
}

void MovementComponent::DirectionControl(int controllerIndex)
{
	// 오른쪽 스틱 입력값
	const framework::Vector2D rawThumbRight = InputManager::GetPadAxisRightThumb(controllerIndex);

	// 보정된 오른쪽 스틱의 입력값
	framework::Vector2D thumbRight = { max(-1, rawThumbRight.x), max(-1, (-1) * rawThumbRight.y) };

	// 오른쪽 스틱에 입력값이 있는 경우에만 방향을 바꿔준다
	if (thumbRight.x != 0.f || thumbRight.y != 0.f)
		m_LookDir = { thumbRight.x, thumbRight.y };

	// 바라보는 방향의 단위 벡터를 구한다
	m_LookDir.Normalize();
}

void MovementComponent::ApplyFriction()
{
	// 마찰 계수 (0 ~ 1 사이의 값)
	// constexpr float frictionCoefficient = 0.995f; // 느리게 멈춤
	constexpr float frictionCoefficient = 0.95f; // 빠르게 멈춤 (등속도 처럼)

	// 마찰력 적용
	m_Velocity.x *= frictionCoefficient;
	m_Velocity.y *= frictionCoefficient;
}

void MovementComponent::AddForce(framework::Vector2D vec)
{
	m_Velocity += vec;
}

/// <summary>
/// 컨트롤러 기능 테스트
/// </summary>
/// <param name="controllerIndex"></param>
void MovementComponent::XInputTestFunc(int controllerIndex)
{
	/*
	/// A B X Y Input Test
	if (InputManager::GetInstance()->IsPadButtonPush(controllerIndex, GamePadButtonCode::A))
		std::cout << std::to_string(controllerIndex + 1) << "P의 A 버튼 PUSH" << std::endl;
	else if (InputManager::GetInstance()->IsPadButtonEnd(controllerIndex, GamePadButtonCode::A))
		std::cout << std::to_string(controllerIndex + 1) << "P의 A 버튼 END" << std::endl;

	if (InputManager::GetInstance()->IsPadButtonPush(controllerIndex, GamePadButtonCode::B))
		std::cout << std::to_string(controllerIndex + 1) << "P의 B 버튼 PUSH" << std::endl;
	else if (InputManager::GetInstance()->IsPadButtonEnd(controllerIndex, GamePadButtonCode::B))
		std::cout << std::to_string(controllerIndex + 1) << "P의 B 버튼 END" << std::endl;

	/// L1 R1 Input Test
	if (InputManager::GetInstance()->IsPadButtonPush(controllerIndex, GamePadButtonCode::L1))
		std::cout << std::to_string(controllerIndex + 1) << "P의 L1 버튼 PUSH" << std::endl;
	else if (InputManager::GetInstance()->IsPadButtonEnd(controllerIndex, GamePadButtonCode::L1))
	{
		std::cout << std::to_string(controllerIndex + 1) << "P의 L1 버튼 END" << std::endl;
		InputManager::SetControllerVib(controllerIndex, 0, 0);
	}

	if (InputManager::GetInstance()->IsPadButtonPush(controllerIndex, GamePadButtonCode::R1))
		std::cout << std::to_string(controllerIndex + 1) << "P의 R1 버튼 PUSH" << std::endl;
	else if (InputManager::GetInstance()->IsPadButtonEnd(controllerIndex, GamePadButtonCode::R1))
	{
		std::cout << std::to_string(controllerIndex + 1) << "P의 R1 버튼 END" << std::endl;
		InputManager::SetControllerVib(controllerIndex, 0, 0);
	}

	/// L3 R3 Input Test
	if (InputManager::GetInstance()->IsPadButtonPush(controllerIndex, GamePadButtonCode::L3))
		std::cout << std::to_string(controllerIndex + 1) << "P의 L3 PUSH" << std::endl;
	else if (InputManager::GetInstance()->IsPadButtonEnd(controllerIndex, GamePadButtonCode::L3))
		std::cout << std::to_string(controllerIndex + 1) << "P의 L3 END" << std::endl;

	if (InputManager::GetInstance()->IsPadButtonPush(controllerIndex, GamePadButtonCode::R3))
		std::cout << std::to_string(controllerIndex + 1) << "P의 R3 PUSH" << std::endl;
	else if (InputManager::GetInstance()->IsPadButtonEnd(controllerIndex, GamePadButtonCode::R3))
		std::cout << std::to_string(controllerIndex + 1) << "P의 R3 END" << std::endl;

	/// START BACK Input Test
	if (InputManager::GetInstance()->IsPadButtonPush(controllerIndex, GamePadButtonCode::START))
		std::cout << std::to_string(controllerIndex + 1) << "P의 START PUSH" << std::endl;
	else if (InputManager::GetInstance()->IsPadButtonEnd(controllerIndex, GamePadButtonCode::START))
		std::cout << std::to_string(controllerIndex + 1) << "P의 START END" << std::endl;

	if (InputManager::GetInstance()->IsPadButtonPush(controllerIndex, GamePadButtonCode::BACK))
		std::cout << std::to_string(controllerIndex + 1) << "P의 BACK PUSH" << std::endl;
	else if (InputManager::GetInstance()->IsPadButtonEnd(controllerIndex, GamePadButtonCode::BACK))
		std::cout << std::to_string(controllerIndex + 1) << "P의 BACK END" << std::endl;

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

				여기서부터 Movement 관련 함수 코드이고 위의 내용은 Input Test Code

	-------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------*/

	MoveControl(controllerIndex);
	DirectionControl(controllerIndex);
}

void MovementComponent::Update(const float deltaTime)
{
	// 1P ~ 4P까지의 컨트롤러 입력을 받아온다
	if (m_pOwner->GetName() == L"Player1")
		XInputTestFunc(0);
	else if (m_pOwner->GetName() == L"Player2")
		XInputTestFunc(1);
	else if (m_pOwner->GetName() == L"Player3")
		XInputTestFunc(2);
	else if (m_pOwner->GetName() == L"Player4")
		XInputTestFunc(3);


	/// 입력 방향이 바뀌면 빨리 반대로 갈 수 있도록
	if (std::signbit(m_PrevMoveDir.x * m_MoveDir.x))
		m_Velocity.x = 0.f;
	if (std::signbit(m_PrevMoveDir.y * m_MoveDir.y))
		m_Velocity.y = 0.f;

	/// 1번 후보 (이걸로 하는게 맞는듯,,,?)
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

	/// 이동 입력 없으면 마찰력 작용
	if (m_MoveDir == framework::Vector2D::Zero())
		ApplyFriction();

	// 최종 속도에 따른 플레이어 이동
	m_pOwner->GetRootComponent()->AddRelativeLocation(m_Velocity * deltaTime);

	/// Transition에 정보 전달
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