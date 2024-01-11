#pragma once

#include <vector>

class FSM;
class FSMTransition;

/// <summary>
///	인스턴스화할 FSMState의 원형
///	전환 조건들을 가지고 있으며, 가진 전환 조건들을 체크한다.
/// </summary>
class FSMState
{
protected:
	FSM* m_pParentFSM = nullptr;

	std::wstring m_StateName;
	std::vector<FSMTransition*> m_Transitions;
	framework::Vector2D m_Direction = {0.f, 1.f};	// 애니메이션 방향

public:
	virtual ~FSMState() { }

public:
	void SetParentFSM(FSM* parentFSM);

	virtual void SetDefaultAnimationName() { };	// DefaultState에서 override
	std::wstring GetName() const { return m_StateName; }
	void SetName(const std::wstring name) { m_StateName = name; }
	void AddTransition(FSMTransition* fsmStransiton) { m_Transitions.push_back(fsmStransiton); }

public:
	virtual bool CheckTransition();
	virtual void OnEnter() abstract;
	virtual void OnStay() abstract;
	virtual void OnExit() abstract;
};

