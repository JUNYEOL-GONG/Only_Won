#pragma once

#include "Component.h"

class FSM;

/// <summary>
/// FSM을 오브젝트에 붙이기 위한 Component
/// FSM 구조는 다음과 같다.
/// GameObject
/// -FSMComponent(!!)
///  -FSM
///   -FSMState
///    -FSMTransition
/// 
/// EventManager를 통해서 AnimationComponent와 MovementComponent의 정보를 받아오고 조작한다.
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

