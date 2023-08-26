#pragma once

class TimeManager
{
public:
	TimeManager();

public:
	void Initialize();
	void Update();
	float GetDeltaTime() const { return m_DeltaTime; }
	float GetFrameRate() const;

/// <summary>
/// 시간 경과 관리 정적 함수들
/// </summary>
public:
	static float GetElapsedTime() { return m_ElapsedTime; }
	static void StopTime() { m_bIsPaused = true; }
	static void PlayTime() { m_bIsPaused = false; }
	static void SwitchPause() { m_bIsPaused = !m_bIsPaused; }
	static void ResetTime() { m_ElapsedTime = 0.f; }
	static bool GetIsPaused() { return m_bIsPaused; }

private:
	LARGE_INTEGER m_PreviousTime;
	LARGE_INTEGER m_CurrentTime;
	LARGE_INTEGER m_Frequency;
	float m_DeltaTime;

private:
	static bool m_bIsPaused;
	static float m_ElapsedTime;
};