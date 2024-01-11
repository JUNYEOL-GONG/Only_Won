#include "pch.h"
#include "TimeManager.h"

float TimeManager::m_ElapsedTime = 0.f;
bool TimeManager::m_bIsPaused = false;

TimeManager::TimeManager()
	:m_PreviousTime{}, m_CurrentTime{}, m_Frequency{}, m_DeltaTime(0.f)
{
}

void TimeManager::Initialize()
{
	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_PreviousTime);
	QueryPerformanceCounter(&m_CurrentTime);
}

void TimeManager::Update()
{
	m_PreviousTime = m_CurrentTime;
	QueryPerformanceCounter(&m_CurrentTime);
	m_DeltaTime = static_cast<float>(m_CurrentTime.QuadPart - m_PreviousTime.QuadPart) / static_cast<float>(m_Frequency.QuadPart);

    // TimeManager�� �������� ���� ��쿡�� ��� �ð��� �����Ѵ�.
    if (!m_bIsPaused)
		m_ElapsedTime += m_DeltaTime; // ��� �ð� ���

#ifdef _DEBUG
    if (m_DeltaTime > (1.f / 60.f))
        m_DeltaTime = (1.f / 60.f);
#endif
}

float TimeManager::GetFrameRate() const
{
    static int frameCnt = -1;
    static float timeElapsed = 0.f;
    static float fps = 0;;

    frameCnt++;
    if (frameCnt == 0)
        return 0;

	timeElapsed += m_DeltaTime;

    if (timeElapsed >= 1.0f)
    {
        fps = static_cast<float>(frameCnt);
        frameCnt = 0;
        timeElapsed -= 1.0f;
    }
    return fps;
}