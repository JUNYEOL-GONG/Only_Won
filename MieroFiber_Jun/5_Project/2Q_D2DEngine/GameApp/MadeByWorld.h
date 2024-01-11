#pragma once
#include "../Engine/EventListener.h"
#include "../Engine/World.h"

class MadeByUIObject;

class MadeByWorld
	: public World
	, public EventListener
{
private:
	MadeByUIObject* m_pMainUIObject = nullptr;

public:
	bool Initialize() final;
	void Finalize() final;

	void Enter() final;
	void Exit() final;

	void HandleEvent(Event* event) final;
};

