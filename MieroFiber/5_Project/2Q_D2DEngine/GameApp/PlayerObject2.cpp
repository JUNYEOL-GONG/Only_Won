#include "framework.h"
#include "PlayerObject2.h"

#include "../Engine/TextureComponent.h"
#include "../Engine/PlayerMovement.h"
#include "../Engine/DirectionComponent.h"
#include "../Engine/BoxCollider2D.h"

#include "ProjectileObject.h"

#include "../Engine/World.h"

#include <vector>

PlayerObject2::~PlayerObject2()
{
}

bool PlayerObject2::Initialize()
{
	/// TextrueComponent
	m_pTextureComponent = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pTextureComponent->SetTextureAsset(L"../Resource/racoon.png", L"racoonTexture");
	m_pTextureComponent->SetRelativeLocation(ScreenWidth / 2.f + 400.f, ScreenHeight / 2.f);
	SetRootComponent(m_pTextureComponent);

	/// PlayerMovement
	m_pMovementComponent = CreateComponent<PlayerMovement>(L"PlayerMovement");

	/// DirectionComponent
	m_pDirectionComponent = CreateComponent<DirectionComponent>(L"DirectionComponent");
	m_pDirectionComponent->SetColor(D2D1::ColorF::Yellow);
	m_pDirectionComponent->AttachToComponent(m_pTextureComponent);

	/// BoxCollider2D
	m_pBoxCollider2D = CreateComponent<BoxCollider2D>(L"BoxCollider2D");
	m_pBoxCollider2D->SetRelativeLocation(0.f, 25.f);
	m_pBoxCollider2D->SetExtend(30.f, 25.f);
	m_pBoxCollider2D->AttachToComponent(m_pTextureComponent);

	m_pBoxCollider2DPart = CreateComponent<BoxCollider2D>(L"Player_Part");
	m_pBoxCollider2DPart->SetRelativeLocation(0.f, 30.f);
	m_pBoxCollider2DPart->SetExtend(10.f, 15.f);
	m_pBoxCollider2DPart->AttachToComponent(m_pTextureComponent);

	const bool res = GameObject::Initialize();
	assert(res);

	return true;
}

void PlayerObject2::Update(const float deltaTime)
{
	// Test Code
	if (this->GetRootComponent()->GetWorldLocation().x > ScreenWidth || this->GetRootComponent()->GetWorldLocation().x < 0 ||
		this->GetRootComponent()->GetWorldLocation().y > ScreenHeight || this->GetRootComponent()->GetWorldLocation().y < 0)
	{
		this->SetGameObjectLocation(ScreenWidth / 2.f, ScreenHeight / 2.f);
	}

	GameObject::Update(deltaTime);

	// 아이템 투척
	if (InputManager::GetInstance()->IsRightTriggerPush(1) ||
		InputManager::GetInstance()->IsLeftTriggerPush(1) ||
		InputManager::GetInstance()->IsPadButtonPush(1, GamePadButtonCode::X))
	{
		auto* projectile = GetOwnerWorld()->CreateGameObject<ProjectileObject>(L"Projectile_Clone", GROUP_TYPE::ITEM);

		// 아이템의 소유자 설정
		//projectile->SetOwnerPlayer(this);

		// 속도 변경
		projectile->SetSpeed(500.f);

		// 방향 설정
		const framework::Vector2D tempLookDir = dynamic_cast<PlayerMovement*>(GetComponent(L"PlayerMovement"))->GetLookDir();
		projectile->SetDirection(tempLookDir);

		// 위치 설정
		const framework::Vector2D pos = GetRootComponent()->GetRelativeLocation();
		constexpr float distance = 65.f;
		const framework::Vector2D distVec = tempLookDir * distance;
		projectile->SetPosition(pos + distVec);

		projectile->Initialize();
	}
}

void PlayerObject2::OnCollisionEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{
	if (otherCollision->GetOwner()->GetName() == L"TrainObstacle_Clone")
		InputManager::SetControllerVib(1, 65535, 65535);
}

void PlayerObject2::OnCollisionStay(Collider2D* otherCollision)
{
	if (otherCollision->GetOwner()->GetName() == L"TrainObstacle_Clone")
		InputManager::SetControllerVib(1, 65535, 65535);
}

void PlayerObject2::OnCollisionExit(Collider2D* otherCollision)
{
	if (otherCollision->GetOwner()->GetName() == L"TrainObstacle_Clone")
		InputManager::SetControllerVib(1, 0, 0);
}

void PlayerObject2::OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{
	InputManager::SetControllerVib(1, 65535, 65535);
}

void PlayerObject2::OnTriggerStay(Collider2D* otherCollision)
{
	InputManager::SetControllerVib(1, 65535, 65535);
}

void PlayerObject2::OnTriggerExit(Collider2D* otherCollision)
{
	InputManager::SetControllerVib(1, 0, 0);
}