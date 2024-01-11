#pragma once

#include "RenderComponent.h"

#include <map>

enum class COLLISION_TYPE
{
	COLLISION = 0,
	TRIGGER,
	NONE,

	END
};

struct AABB;
struct CC;

/// <summary>
/// AABB 충돌
/// </summary>
struct AABB
{
	framework::Vector2D center;	// 중심점
	framework::Vector2D extend; // 확장 범위

	// AABB - AABB Collision
	bool Check_AABB_AABB(const AABB& other) const;
	// AABB - CC Collision
	bool Check_AABB_CC(const CC& other) const;
};

/// <summary>
/// Circle 충돌
/// </summary>
struct CC
{
	framework::Vector2D center;	// 중심점
	float radius;				// 반지름

	// CC - CC Collision
	bool Check_CC_CC(const CC& other) const;
	// CC - AABB Collision
	bool Check_CC_AABB(const AABB& other) const;
};

/// <summary>
/// 물체의 충돌을 담당하는 컴포넌트의 기반 클래스
/// </summary>
class Collider2D :
    public RenderComponent
{
protected:
	float m_Width = 0.f;
	float m_Height = 0.f;
	D2D1::ColorF m_Color = D2D1::ColorF::Green;

public:
	COLLISION_TYPE m_CollisionType = COLLISION_TYPE::COLLISION;
	bool m_IsCollision = false;
	bool m_IsTrigger = false;

	static UINT m_NextID;			// 다음 Collider2D의 ID
	UINT m_CollisionCount = 0;		// 충돌중인 다른 충돌체 갯수
	UINT m_ID = 0;					// Collider2D의 ID

public:
	Collider2D();
	~Collider2D() override;

public:
	void SetWidth(const float width) { m_Width = width; }
	void SetHeight(const float height) { m_Height = height; }
	void SetColor(D2D1::ColorF color) { m_Color = color; }

public:
	COLLISION_TYPE GetCollisionType() const { return m_CollisionType; }
	void SetCollisionType(COLLISION_TYPE type) { m_CollisionType = type; }

	void TurnOn_IsCollision(Collider2D* otherCollision) { m_IsCollision = true; otherCollision->m_IsCollision = true; }
	void TurnOff_IsCollision(Collider2D* otherCollision) { m_IsCollision = false; otherCollision->m_IsCollision = false; }

	void TurnOn_IsTrigger(Collider2D* otherCollision) { m_IsTrigger = true; otherCollision->m_IsTrigger = true; }
	void TurnOff_IsTrigger(Collider2D* otherCollision) { m_IsTrigger = false; otherCollision->m_IsTrigger = false; }
public:
	bool Initialize() override;
	void Update(const float deltaTime) override;
	void Render(ID2D1RenderTarget* pRenderTarget) override;

public:
	virtual bool CheckIntersect(Collider2D* other) const abstract;

public:
	UINT GetID() const { return m_ID; }

public:
	virtual void OnCollisionEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter) {}
	virtual void OnCollisionStay(Collider2D* otherCollision) {}
	virtual void OnCollisionExit(Collider2D* otherCollision) {}

	virtual void OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter) {}
	virtual void OnTriggerStay(Collider2D* otherCollision) {}
	virtual void OnTriggerExit(Collider2D* otherCollision) {}

public:
	virtual void ProcessBlock(Collider2D* otherCollision) abstract;
};