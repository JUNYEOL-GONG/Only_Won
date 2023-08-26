#pragma once

#include <unordered_map>

class GameObject;
class FSMComponent;
class FSMState;
class AnimationComponent;

/// <summary>
/// 오브젝트에 맞춰 인스턴스화할 FSM의 원형
///	모든 상태 중에서 현재 상태와 다음 상태를 가지고 전환, 현재 상태를 Update 해준다.
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