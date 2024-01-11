#pragma once
#include "ItemObject.h"

class PlayerMovement;
class TextureComponent;
class AnimationComponent;
class BoxCollider2D;

class SnareItem
	: public ItemObject
{
private:
	BoxCollider2D* m_pCollider = nullptr;
	AnimationComponent* m_pEffectAnimation = nullptr;	// 동작시 애니메이션 재생

	PlayerObject* m_pPlayer = nullptr;
	PlayerMovement* m_pPlayerMovement = nullptr;

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