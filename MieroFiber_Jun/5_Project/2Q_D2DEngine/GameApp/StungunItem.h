#pragma once

#include "ProjectileObject.h"

class AnimationComponent;
class PlayerMovement;

class StungunItem :
	public ProjectileObject
{
private:
	AnimationComponent* m_pEffectAnimation = nullptr;

private:
	float m_stunGunLifeTime = 0.f;
	float m_stunGunMaxLifeTime = 0.22f;

public:
	~StungunItem() final;

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