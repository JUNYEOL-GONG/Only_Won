#pragma once
#include "ItemObject.h"

class AnimationComponent;
class BoxCollider2D;
class PlayerObject;

class TrapItem :
    public ItemObject
{
private:
	BoxCollider2D* m_pCollider = nullptr;
	PlayerObject* m_pAttackedPlayer = nullptr;

	bool m_Active = false;
	bool m_bAffected = false;

	float m_DelayTime = 0.f;
	float m_ProgressTime = 0.f;

public:
	bool Initialize() final;
	void Update(const float deltaTime) final;

public:
	void OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter) final;
};