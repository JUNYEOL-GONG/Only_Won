#pragma once

#include "Component.h"

/// <summary>
/// �̵��� ���� ������Ʈ
/// MoveDirection�� LookDirection���� �̵��Ѵ�.
/// </summary>
class MovementComponent
	: public Component
{
private:
	float m_MoveSpeed = 300.f;								// �ӷ�
	float m_AccelSpeed = 2000.f;							// ���ӷ�
	framework::Vector2D m_Velocity = { 0.f, 0.f };		// �ӵ�

private:
	bool isSliding = false;
	float slidingTime = 0.f;

	bool isKnockback = false;
	float knockbackTime = 0.f;

private:
	framework::Vector2D m_PrevMoveDir = { 0.f, 0.f };
	framework::Vector2D m_MoveDir = { 0.f, 0.f };		// �̵� ����
	framework::Vector2D m_LookDir = { 1.f, 0.f };		// �ٶ󺸴� ����

public:
	void MoveControl(int controllerIndex);
	void DirectionControl(int controllerIndex);

	void ApplyFriction();
	void AddForce(framework::Vector2D vec);

	framework::Vector2D GetMoveDir() const { return m_MoveDir; }
	framework::Vector2D GetLookDir() const { return m_LookDir; }
	framework::Vector2D GetVelocity() const { return m_Velocity; }

	/// <summary>
	/// XInput API�� ��� ���� �� ��Ʈ�ѷ� �Է��� ���� �÷��̾� �̵� �Լ�
	/// ���߿� ��ư ��� ���� �ڵ�� Movement Component�� �ƴ� �ٸ� ������ (�÷��̾�?) ���� �����Ѵ�
	/// </summary>
	/// <param name="controllerIndex"></param>
	void XInputTestFunc(int controllerIndex);

public:
	void Update(const float deltaTime) final;
};