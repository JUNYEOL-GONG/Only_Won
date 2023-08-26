#pragma once
#include "../Engine/EventListener.h"
#include "../Engine/World.h"

/// <summary>
/// 게임 방법 알려주는 월드
/// 게임 규칙 알려주고 키보드랑 컨트롤러 조작법도 알려줌
/// </summary>

class InstructionUIObject;

class InstructionWorld
	: public World
	, public EventListener
{
private:
	InstructionUIObject* m_pInstructionUIObject = nullptr;

public:
	bool Initialize() final;
	void Finalize() final;

	void Enter() final;
	void Exit() final;

	void HandleEvent(Event* event) final;
};

