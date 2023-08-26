#pragma once

#include "../Engine/FSMTransition.h"
#include "../Engine/EventListener.h"

class PlayerTransition
	: public FSMTransition
	, public EventListener
{
protected:
	framework::Vector2D m_MoveDirection = { 0.f, 0.f };
	framework::Vector2D m_LookDirection = { 0.f, 0.f };

public :
	std::pair<std::wstring, bool> operator()() override abstract;
	void HandleEvent(Event* event) final;
};

class IdleToMove : public PlayerTransition
{
public:
	std::pair<std::wstring, bool> operator()() final
	{
		
		if(m_MoveDirection.Length() > 0.f)
		{
			if (m_LookDirection.x >= 0.f)
				return { L"MOVE", false };
			else
				return { L"MOVE", true };
		}
		else
		{
			if (m_LookDirection.x >= 0.f)
				return { L"", false };
			else
				return { L"", true };
		}
	}
};

class MoveToIdle : public PlayerTransition
{
public:
	std::pair<std::wstring, bool> operator()() final
	{
		if(m_MoveDirection.Length() == 0.f)
		{
			if(m_LookDirection.x >= 0.f)
				return { L"IDLE", false };
			else
				return { L"IDLE", true };
		}
		else
		{
			if (m_LookDirection.x >= 0.f)
				return { L"", false };
			else
				return { L"", true };
		}
	}
};