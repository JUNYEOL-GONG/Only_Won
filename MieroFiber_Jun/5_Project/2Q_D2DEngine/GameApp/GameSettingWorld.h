#pragma once
#include "../Engine/EventListener.h"
#include "../Engine/World.h"

class GameSettingUIObject;

class GameSettingWorld
	: public World
	, public EventListener
{
private:
	GameSettingUIObject* m_pGameSettingUIObject = nullptr;

public:
	bool Initialize() final;
	void Finalize() final;

	void Enter() final;
	void Exit() final;

	void HandleEvent(Event* event) final;
};