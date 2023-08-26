#pragma once

#include <vector>

class FSM;
class FSMTransition;

/// <summary>
///	�ν��Ͻ�ȭ�� FSMState�� ����
///	��ȯ ���ǵ��� ������ ������, ���� ��ȯ ���ǵ��� üũ�Ѵ�.
/// </summary>
class FSMState
{
protected:
	FSM* m_pParentFSM = nullptr;

	std::wstring m_StateName;
	std::vector<FSMTransition*> m_Transitions;
	framework::Vector2D m_Direction = {0.f, 1.f};	// �ִϸ��̼� ����

public:
	virtual ~FSMState() { }

public:
	void SetParentFSM(FSM* parentFSM);

	virtual void SetDefaultAnimationName() { };	// DefaultState���� override
	std::wstring GetName() const { return m_StateName; }
	void SetName(const std::wstring name) { m_StateName = name; }
	void AddTransition(FSMTransition* fsmStransiton) { m_Transitions.push_back(fsmStransiton); }

public:
	virtual bool CheckTransition();
	virtual void OnEnter() abstract;
	virtual void OnStay() abstract;
	virtual void OnExit() abstract;
};

