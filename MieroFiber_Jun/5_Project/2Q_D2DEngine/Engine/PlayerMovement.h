#pragma once

#include "Component.h"

/// <summary>
/// �̵��� ���� ������Ʈ
/// MoveDirection�� LookDirection���� �̵��Ѵ�.
/// </summary>
class PlayerMovement
	: public Component
{
private:
	bool m_IsActive = true;									// Ȱ��ȭ ����

private:
	framework::Vector2D m_OriginalVelocity = { 0.f, 0.f };		// �������� �ӵ�
	framework::Vector2D m_ExternalVelocity = { 0.f, 0.f };		// �ܺ� �ӵ�
	framework::Vector2D m_TotalVelocity = { 0.f, 0.f };			// ���� �ӵ�
	framework::Vector2D m_OriginalAccel = { 0.f, 0.f };			// �������� ���ӵ�
	framework::Vector2D m_ExternalAccel = { 0.f, 0.f };			// �ܺ� ���ӵ�
	framework::Vector2D m_TotalAccel = { 0.f, 0.f };			// ���� ���ӵ�
	float m_OriginalAccelSpeed = 3000.f;							// �������� ���ӷ�
	float m_ExternalAccelSpeed = 3000.f;							// �ܺ� ���ӷ�
	float m_OriginalVelocityLimit = 400.f;							// �������� �ӵ� ����
	float m_ExternalVelocityLimit = 1500.f;							// �ܺ� �ӵ� ����
	const float m_DefaultVelocityLimit = m_OriginalVelocityLimit;	// ���� �ӵ�

private:
	framework::Vector2D m_PrevMoveDir = { 0.f, 0.f };	// ���� �̵� ����
	framework::Vector2D m_MoveDir = { 0.f, 0.f };		// �̵� ����
	framework::Vector2D m_LookDir = { 0.f, 1.f };		// �ٶ󺸴� ����

private:
	bool m_IsReverseMove = false;		// �̵� ���� ����
	float m_ReverseCoefficient = 1.f;		// �̵� ���� ���`

public:
	void SetActive(bool isActive) { m_IsActive = isActive; }
	bool IsActive() const { return m_IsActive; }

	void KeyDirControl();
	void TotalDirControl(int controllerIndex);
	void MoveDirControl(int controllerIndex);
	void LookDirControl(int controllerIndex);
	void PlayerMove(float deltaTime);
	void OriginalFriction(float deltaTime);
	void ExternalFriction(float deltaTime);

	framework::Vector2D GetMoveDir() const { return m_MoveDir; }
	framework::Vector2D GetLookDir() const { return m_LookDir; }

	framework::Vector2D GetOriginalVelocity() const { return m_OriginalVelocity; }
	framework::Vector2D GetExternalVelocity() const { return m_ExternalVelocity; }
	framework::Vector2D GetTotalVelocity() const { return m_TotalVelocity; }

	// �ܺο��� �ӵ� ������ ���
	void SetOriginalVelocityLimit(float rate)
	{
		// ex) 2��
		if (rate > 1.f)
			m_OriginalVelocityLimit = m_OriginalVelocityLimit + m_OriginalVelocityLimit * (rate - 1.f);
		// ex) 0.5��
		else if (rate < 1.f)
			m_OriginalVelocityLimit = m_OriginalVelocityLimit - m_OriginalVelocityLimit * (1.f - rate);

		if(m_OriginalVelocityLimit > m_DefaultVelocityLimit * 2)
		{
			m_OriginalVelocityLimit = m_DefaultVelocityLimit;
		}
	}
	float GetOriginalVelocityLimitRate() const { return m_OriginalVelocityLimit/m_DefaultVelocityLimit; }
	void SetExternalVelocityLimit(float rate) { m_ExternalVelocityLimit *= rate; }
	float GetExternalVelocityLimit() const { return m_ExternalVelocityLimit; }

	void ResetVelocityLimit() { m_OriginalVelocityLimit = m_DefaultVelocityLimit; }
	void ResetExternalVelocity() { m_ExternalVelocity = { 0.f, 0.f }; }

	bool GetIsReverseMove() const { return m_IsReverseMove; }
	void SetIsReverseMove(bool isReverseMove) { m_IsReverseMove = isReverseMove; }

	void KnockBack(framework::Vector2D dir, float power);
	void ReverseMove();

public:
	void Update(const float deltaTime) final;
};