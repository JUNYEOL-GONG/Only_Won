#pragma once

#include "Component.h"

/// <summary>
/// 이동을 위한 컴포넌트
/// MoveDirection과 LookDirection으로 이동한다.
/// </summary>
class PlayerMovement
	: public Component
{
private:
	bool m_IsActive = true;									// 활성화 여부

private:
	framework::Vector2D m_OriginalVelocity = { 0.f, 0.f };		// 오리지널 속도
	framework::Vector2D m_ExternalVelocity = { 0.f, 0.f };		// 외부 속도
	framework::Vector2D m_TotalVelocity = { 0.f, 0.f };			// 최종 속도
	framework::Vector2D m_OriginalAccel = { 0.f, 0.f };			// 오리지널 가속도
	framework::Vector2D m_ExternalAccel = { 0.f, 0.f };			// 외부 가속도
	framework::Vector2D m_TotalAccel = { 0.f, 0.f };			// 최종 가속도
	float m_OriginalAccelSpeed = 3000.f;							// 오리지널 가속력
	float m_ExternalAccelSpeed = 3000.f;							// 외부 가속력
	float m_OriginalVelocityLimit = 400.f;							// 오리지널 속도 제한
	float m_ExternalVelocityLimit = 1500.f;							// 외부 속도 제한
	const float m_DefaultVelocityLimit = m_OriginalVelocityLimit;	// 원본 속도

private:
	framework::Vector2D m_PrevMoveDir = { 0.f, 0.f };	// 이전 이동 방향
	framework::Vector2D m_MoveDir = { 0.f, 0.f };		// 이동 방향
	framework::Vector2D m_LookDir = { 0.f, 1.f };		// 바라보는 방향

private:
	bool m_IsReverseMove = false;		// 이동 반전 여부
	float m_ReverseCoefficient = 1.f;		// 이동 반전 계수`

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

	// 외부에서 속도 조절시 사용
	void SetOriginalVelocityLimit(float rate)
	{
		// ex) 2배
		if (rate > 1.f)
			m_OriginalVelocityLimit = m_OriginalVelocityLimit + m_OriginalVelocityLimit * (rate - 1.f);
		// ex) 0.5배
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