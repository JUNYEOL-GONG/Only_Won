#pragma once

#include <unordered_map>

class GameObject;
class FSMComponent;
class FSMState;
class AnimationComponent;

/// <summary>
/// ������Ʈ�� ���� �ν��Ͻ�ȭ�� FSM�� ����
///	��� ���� �߿��� ���� ���¿� ���� ���¸� ������ ��ȯ, ���� ���¸� Update ���ش�.
/// </summary>
class FSM
{
protected:
	std::unordered_map<std::wstring, FSMState*> m_States;
	FSMState* m_pCurState = nullptr;
	FSMState* m_pNextState = nullptr;

public:
	virtual ~FSM() { }

public:
	void AddState(const std::wstring name, FSMState* state);
	void SetNextState(const std::wstring& stateName) { m_pNextState = m_States.find(stateName)->second; }
	void SetNextState(FSMState* nextState) { m_pNextState = nextState; }
	FSMState* GetCurState() const { return m_pCurState; }
	FSMState* FindState(const std::wstring& name) const { return m_States.find(name)->second; }
	void SetDefaultState(FSMState* defaultState);

public:
	void Update(const float deltaTime);
};