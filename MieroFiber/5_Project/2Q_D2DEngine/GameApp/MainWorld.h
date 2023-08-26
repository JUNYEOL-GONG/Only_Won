#pragma once
#include "../Engine/EventListener.h"
#include "../Engine/World.h"

/// <summary>
/// 게임의 메인 화면
/// 게임 시작, 게임 방법, 제작, 끝내기 버튼이 있음
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