#pragma once

#include "../Engine/EventListener.h"
#include "../Engine/World.h"

class PopUpUIObject2;
class PlayerObject1;
class UIObject;

class ChaeWorld
	: public World
	, public EventListener
{
public:
	UIObject* m_pUIObject;
	PopUpUIObject* m_pPopUpUIObject1;
	PopUpUIObject2* m_pPopUpUIObject2;
	PlayerObject1* m_pPlayerObject1 = nullptr;

public:
	ChaeWorld();

	bool Initialize() final;
	void Finalize() final;

	void Enter() final;
	void Exit() final;

public:
	void HandleEvent(Event* event) final;
};