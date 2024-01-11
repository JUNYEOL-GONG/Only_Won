#pragma once
#include "../Engine/EventListener.h"
#include "../Engine/World.h"

/// <summary>
/// ������ ���� ȭ��
/// ���� ����, ���� ���, ����, ������ ��ư�� ����
/// </summary>

class MainUIObject;

class MainWorld
	: public World
	, public EventListener
{
private:
	MainUIObject* m_pMainUIObject = nullptr;

public:
	bool Initialize() final;
	void Finalize() final;

	void Enter() final;
	void Exit() final;

	void HandleEvent(Event* event) final;
};