#pragma once

#include "World.h"

#include <vector>

class World;
class UIManager;

class WorldManager
{
public:
	WorldManager();

public:
	void Initialize();
	void Update(float deltaTime);
	void Render();
	void Finalize();

public:
	void RegisterWorld(World* world, WORLD_TYPE type);
	void SetDefaultWorld(World* world) { m_currentWorld = world; }
	World* GetCurWorld() const { return m_currentWorld; }
	void ChangeWorld(WORLD_TYPE world);
	void GoToNextWorld();
	void GoToPrevWorld();

private:
	std::vector<World*> m_worlds;

	UIManager* m_pUIManager = nullptr;

	World* m_currentWorld = nullptr;
	World* m_prevWorld = nullptr;

	bool m_bPaused = false;
};