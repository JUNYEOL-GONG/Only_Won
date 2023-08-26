#pragma once

#include "../Engine/FSMTransition.h"
#include "../Engine/EventListener.h"

class PlayerTransition2
	: public FSMTransition
	, public EventListener
{
protected:
	framework::Vector2D m_MoveDirection = { 0.f, 0.f };
	framework::Vector2D m_LookDirection = { 0.f, 0.f };

	int m_HP = 0;

public:
	std::pair<std::wstring, framework::Vector2D> operator()() override abstract;
	void HandleEvent(Event* event) final;
};

class IdleToMove2 : public PlayerTransition2
{
public:
	std::pair<std::wstring, framework::Vector2D> operator()() final
	{
		if (m_MoveDirection.Length() > 0.f)
		{
			if (m_LookDirection.y <= -0.8f)
			{
				return { L"MOVE", {0.f, -1.f} };
			}
			else if (m_LookDirection.y >= 0.8f)
			{
				return { L"MOVE", {0.f, 1.f} };
			}
			else
			{
				if (m_LookDirection.x >= 0.f)
					return { L"MOVE", {1.f, 0.f} };
				else
					return { L"MOVE", {-1.f, 0.f} };
			}
		}
		else
		{
			if (m_LookDirection.y <= -0.8f)
			{
				return { L"", {0.f, -1.f} };
			}
			else if (m_LookDirection.y >= 0.8f)
			{
				return { L"", {0.f, 1.f} };
			}
			else
			{
				if (m_LookDirection.x >= 0.f)
					return { L"", {1.f, 0.f} };
				else
					return { L"", {-1.f, 0.f} };
			}
		}
	}
};


class MoveToIdle2 : public PlayerTransition2
{
public:
	std::pair<std::wstring, framework::Vector2D> operator()() final
	{
		if (m_MoveDirection.Length() == 0.f)
		{
			if (m_LookDirection.y <= -0.8f)
			{
				return { L"IDLE", {0.f, -1.f} };
			}
			else if (m_LookDirection.y >= 0.8f)
			{
				return { L"IDLE", {0.f, 1.f} };
			}
			else
			{
				if (m_LookDirection.x >= 0.f)
					return { L"IDLE", {1.f, 0.f} };
				else
					return { L"IDLE", {-1.f, 0.f} };
			}
		}
		else
		{
			if (m_LookDirection.y <= -0.8f)
			{
				return { L"", {0.f, -1.f} };
			}
			else if (m_LookDirection.y >= 0.8f)
			{
				return { L"", {0.f, 1.f} };
			}
			else
			{
				if (m_LookDirection.x >= 0.f)
					return { L"", {1.f, 0.f} };
				else
					return { L"", {-1.f, 0.f} };
			}
		}
	}
};

class StateToStun2 : public PlayerTransition2
{
	std::pair<std::wstring, framework::Vector2D> operator()() final
	{
		if (m_HP > 0)
		{
			if (m_LookDirection.y <= -0.8f)
			{
				return { L"", {0.f, -1.f} };
			}
			else if (m_LookDirection.y >= 0.8f)
			{
				return { L"", {0.f, 1.f} };
			}
			else
			{
				if (m_LookDirection.x >= 0.f)
					return { L"", {1.f, 0.f} };
				else
					return { L"", {-1.f, 0.f} };
			}
		}
		else
		{
			return { L"STUN", {0.f, 1.f} };
		}
	}
};

class StunToIdle2 : public PlayerTransition2
{
	std::pair<std::wstring, framework::Vector2D> operator()() final
	{
		if (m_HP > 0)
		{
			return { L"IDLE", {0.f, 1.f} };
		}
		else
		{
			return { L"", {0.f, 1.f} };
		}
	}
};