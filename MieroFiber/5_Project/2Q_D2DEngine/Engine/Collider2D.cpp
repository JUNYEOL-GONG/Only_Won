#include "pch.h"
#include "Collider2D.h"

#include "GameObject.h"

UINT Collider2D::m_NextID = 0;

bool AABB::Check_AABB_AABB(const AABB& other) const
{
	// self min,max
	const float minBox1X = center.x - extend.x;
	const float maxBox1X = center.x + extend.x;
	const float minBox1Y = center.y - extend.y;
	const float maxBox1Y = center.y + extend.y;

	// other min,max
	const float minBox2X = other.center.x - other.extend.x;
	const float maxBox2X = other.center.x + other.extend.x;
	const float minBox2Y = other.center.y - other.extend.y;
	const float maxBox2Y = other.center.y + other.extend.y;

	// Check for no overlap conditions
	if (maxBox1X < minBox2X || minBox1X > maxBox2X || maxBox1Y < minBox2Y || minBox1Y > maxBox2Y)
		return false;

	return true;
}

bool AABB::Check_AABB_CC(const CC& other) const
{
	// Find the point on the AABB closest to the circle center
	framework::Vector2D closestPoint = other.center;

	// Clamp the closest point to the AABB bounds
	if (closestPoint.x < center.x - extend.x) closestPoint.x = center.x - extend.x;
	if (closestPoint.x > center.x + extend.x) closestPoint.x = center.x + extend.x;
	if (closestPoint.y < center.y - extend.y) closestPoint.y = center.y - extend.y;
	if (closestPoint.y > center.y + extend.y) closestPoint.y = center.y + extend.y;

	// Calculate the distance between the closest point and the circle center
	const framework::Vector2D distanceVec = other.center - closestPoint;

	// If the distance is less than the circle's radius, there is a collision
	return distanceVec.LengthSqrt() <= other.radius * other.radius;
}

bool CC::Check_CC_CC(const CC& other) const
{
	// 두 점 사이의 거리 구하기
	const float distance = framework::Vector2D::Distance(center, other.center);

	// 두 점 사이의 거리가 두 원의 반지름 합보다 작거나 같으면 충돌
	if (distance <= radius + other.radius)
		return true;

	return false;
}

bool CC::Check_CC_AABB(const AABB& other) const
{
	// Find the point on the AABB closest to the circle center
	framework::Vector2D closestPoint = center;

	// Clamp the closest point to the AABB bounds
	if (closestPoint.x < other.center.x - other.extend.x) closestPoint.x = other.center.x - other.extend.x;
	if (closestPoint.x > other.center.x + other.extend.x) closestPoint.x = other.center.x + other.extend.x;
	if (closestPoint.y < other.center.y - other.extend.y) closestPoint.y = other.center.y - other.extend.y;
	if (closestPoint.y > other.center.y + other.extend.y) closestPoint.y = other.center.y + other.extend.y;

	// Calculate the distance between the closest point and the circle center
	const framework::Vector2D distanceVec = center - closestPoint;

	// If the distance is less than the circle's radius, there is a collision
	return distanceVec.LengthSqrt() <= radius * radius;
}

Collider2D::Collider2D()
{
	m_ID = m_NextID++;
}

Collider2D::~Collider2D()
{
}

bool Collider2D::Initialize()
{
	const bool res = SceneComponent::Initialize();
	assert(res);

	return true;
}

void Collider2D::Update(const float deltaTime)
{
	SceneComponent::Update(deltaTime);
}

void Collider2D::Render(ID2D1RenderTarget* pRenderTarget)
{
}