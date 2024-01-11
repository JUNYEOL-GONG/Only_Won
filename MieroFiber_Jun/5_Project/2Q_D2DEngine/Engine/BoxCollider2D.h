#pragma once

#include "Collider2D.h"

class BoxCollider2D :
    public Collider2D
{
public:
	bool Initialize() final;
	void Update(const float deltaTime) final;
	void Render(ID2D1RenderTarget* pRenderTarget) final;

public:
	bool CheckIntersect(Collider2D* otherCollision) const final;

	framework::Vector2D GetCenter() { return { m_collider.center.x,  m_collider.center.y }; }
	framework::Vector2D GetExtend() { return { m_collider.extend.x,  m_collider.extend.y }; }
	void SetExtend(float x, float y);

public:
	void OnCollisionEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter) final;
	void OnCollisionStay(Collider2D* otherCollision) final;
	void OnCollisionExit(Collider2D* otherCollision) final;

	void OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter) final;
	void OnTriggerStay(Collider2D* otherCollision) final;
	void OnTriggerExit(Collider2D* otherCollision) final;

public:
	void ProcessBlock(Collider2D* otherCollision) final;

public:
	AABB m_collider;
};