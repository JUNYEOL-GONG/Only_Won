#pragma once

#include "ItemObject.h"

class BoxCollider2D;

class ProjectileObject :
    public ItemObject
{
protected:
	BoxCollider2D* m_pBoxCollider2D = nullptr;

protected:
	float m_Speed = 600.f;
	float m_Rotation = 0.f;

	float m_LifeTime = 0.f;
	float m_MaxLifeTime = 2.f;

	bool m_bTriggered = false;

	framework::Vector2D m_Position = { 0.f, 0.f };
	framework::Vector2D m_Direction = { 1.0f, 0.f };
	framework::Vector2D m_Velocity = { 1.0f, 0.f };

public:
	~ProjectileObject() override;

public:
	bool Initialize() override;
	void Update(float deltaTime) override;

	void OnCollisionEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter) override;
	void OnCollisionStay(Collider2D* otherCollision) override;
	void OnCollisionExit(Collider2D* otherCollision) override;

	void OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter) override;
	void OnTriggerStay(Collider2D* otherCollision) override;
	void OnTriggerExit(Collider2D* otherCollision) override;

public:
	float GetSpeed() const { return m_Speed; }
	void SetSpeed(const float speed) { m_Speed = speed; }

	float GetRotation() const { return m_Rotation; }
	void SetRotation(const float rotation) { m_Rotation = rotation; }

	framework::Vector2D GetPosition() const { return m_Position; }
	void SetPosition(const framework::Vector2D& position) { m_Position = position; }
	void SetPosition(float x, float y) { m_Position = { x,y }; }

	framework::Vector2D GetDirection() const { return m_Direction; }
	void SetDirection(const framework::Vector2D& direction) { m_Direction = direction; }
};