#pragma once
#include "../Engine/EventListener.h"
#include "../Engine/World.h"

/// <summary>
/// ���� ��� �˷��ִ� ����
/// ���� ��Ģ �˷��ְ� Ű����� ��Ʈ�ѷ� ���۹��� �˷���
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

