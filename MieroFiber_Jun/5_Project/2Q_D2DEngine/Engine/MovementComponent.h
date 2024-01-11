#pragma once

#include "Component.h"

/// <summary>
/// 이동을 위한 컴포넌트
/// MoveDirection과 LookDirection으로 이동한다.
/// </summary>
class MovementComponent
	: public Component
{
private:
	float m_MoveSpeed = 300.f;								// 속력
	float m_AccelSpeed = 2000.f;							// 가속력
	framework::Vector2D m_Velocity = { 0.f, 0.f };		// 속도

private:
	bool isSliding = false;
	float slidingTime = 0.f;

	bool isKnockback = false;
	float knockbackTime = 0.f;

private:
	framework::Vector2D m_PrevMoveDir = { 0.f, 0.f };
	framework::Vector2D m_MoveDir = { 0.f, 0.f };		// 이동 방향
	framework::Vector2D m_LookDir = { 1.f, 0.f };		// 바라보는 방향

public:
	void MoveControl(int controllerIndex);
	void DirectionControl(int controllerIndex);

	void ApplyFriction();
	void AddForce(framework::Vector2D vec);

	framework::Vector2D GetMoveDir() const { return m_MoveDir; }
	framework::Vector2D GetLookDir() const { return m_LookDir; }
	framework::Vector2D GetVelocity() const { return m_Velocity; }

	/// <summary>
	/// XInput API의 사용 예시 및 컨트롤러 입력을 통한 플레이어 이동 함수
	/// 나중에 버튼 사용 예시 코드는 Movement Component가 아닌 다른 곳으로 (플레이어?) 따로 빼야한다
	/// </summary>
	/// <param name="controllerIndex"></param>
	void XInputTestFunc(int controllerIndex);

public:
	void Update(const float deltaTime) final;
};