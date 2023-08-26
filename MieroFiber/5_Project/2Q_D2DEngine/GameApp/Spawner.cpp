#include "framework.h"
#include "Spawner.h"

#include <random>
#include "../Engine/BoxCollider2D.h"

#include "../Engine/World.h"


bool Spawner::Initialize()
{
	m_elapsedTimeList.resize(m_maxCount,-1);

	__super::Initialize();

	return true;
}

void Spawner::Update(const float deltaTime)
{
	__super::Update(deltaTime);
}

bool Spawner::CheckSpawnArea(float x, float y) const
{
	const float minBox1X = x - m_scale;
	const float maxBox1X = x + m_scale;
	const float minBox1Y = y - m_scale;
	const float maxBox1Y = y + m_scale;

	// other min,max
	const float minBox2X = m_randX - m_scale;
	const float maxBox2X = m_randX + m_scale;
	const float minBox2Y = m_randY - m_scale;
	const float maxBox2Y = m_randY + m_scale;

	// Check for no overlap conditions
	if (maxBox1X < minBox2X || minBox1X > maxBox2X || maxBox1Y < minBox2Y || minBox1Y > maxBox2Y)
		return false;

	return true;
}