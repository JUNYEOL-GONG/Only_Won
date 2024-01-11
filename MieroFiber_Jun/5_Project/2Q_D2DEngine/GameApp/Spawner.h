#pragma once

#include "Spawner.h"

#include "../Engine/GameObject.h"

class Spawner
	: public GameObject
{
protected:
	std::vector<D2D1_RECT_F> m_spawnArea = {};
	D2D1_RECT_F m_curSpawnArea = {};

	float m_randX = 0;
	float m_randY = 0;
	float m_spawnTime = 0.f;

	std::vector<GameObject*> m_gameObjectList;
	std::vector<float> m_elapsedTimeList;

	bool m_bCheckAABB = false;

	int m_maxCount = 0;
	int m_scale = 0;

public:
	virtual bool Initialize();
	virtual void Update(const float deltaTime);

public:
	void SetItemSpawnArea(D2D1_RECT_F spawnArea) { m_spawnArea.push_back(spawnArea); }
	void SetItemSpawnTime(float spawnTime) { m_spawnTime = spawnTime; }

	void SetItemCount(int count) { m_maxCount = count; }

	void SetScale(int scale) { m_scale = scale; }

	bool CheckSpawnArea(float x, float y) const;
};

