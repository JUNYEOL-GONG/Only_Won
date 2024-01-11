#pragma once

#include "ProjectileObject.h"

class AnimationComponent;
class PlayerMovement;

class SnowItem :
    public ProjectileObject
{
private:
	PlayerMovement* m_PlayerMovement = nullptr;
	AnimationComponent* m_pEffectAnimation = nullptr;

	bool m_IsAffected = false;
	float m_SlowRate = 0.65f;
	float m_SlowTimer = 0.f;
	float m_SlowTime = 2.f;

public:
	~SnowItem() final;

public:
	bool Initialize() final;
	void Update(float deltaTime) final;

	void OnCollisionEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter) final;
	void OnCollisionStay(Collider2D* otherCollision) final;
	void OnCollisionExit(Collider2D* otherCollision) final;

	void OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter) final;
	void OnTriggerStay(Collider2D* otherCollision) final;
	void OnTriggerExit(Collider2D* otherCollision) final;
};