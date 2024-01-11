#pragma once

#include "Component.h"

class FSM;

/// <summary>
/// FSM�� ������Ʈ�� ���̱� ���� Component
/// FSM ������ ������ ����.
/// GameObject
/// -FSMComponent(!!)
///  -FSM
///   -FSMState
///    -FSMTransition
/// 
/// EventManager�� ���ؼ� AnimationComponent�� MovementComponent�� ������ �޾ƿ��� �����Ѵ�.
/// </summary>
class FSMComponent
	: public Component
{
private:
	FSM* m_FSM = nullptr;

public:
    void AttachFSM(FSM* _FSM);

public:
    void Update(const float deltaTime) final;
};

