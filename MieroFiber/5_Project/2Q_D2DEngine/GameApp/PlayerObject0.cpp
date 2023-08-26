#include "framework.h"
#include "PlayerObject0.h"

#include "PlayerFSM1.h"

#include "../Engine/BoxComponent.h"
#include "../Engine/AnimationComponent.h"
#include "../Engine/PlayerMovement.h"
#include "../Engine/FSMComponent.h"
#include "../Engine/DirectionComponent.h"
#include "../Engine/EventManager.h"

#include <vector>

PlayerObject0::~PlayerObject0()
{
	delete m_pPlayerFSM;
}

bool PlayerObject0::Initialize()
{
	// AnimationComponent
	m_pAnimComponent = CreateComponent<AnimationComponent>(L"AnimationComponent");
	std::vector<framework::FRAME_INFO> playerFrame;
	// Idle
	playerFrame.push_back(framework::FRAME_INFO({ 3, 698, 61, 787 }, 0.1f));
	playerFrame.push_back(framework::FRAME_INFO({ 73, 696, 130, 787 }, 0.1f));
	playerFrame.push_back(framework::FRAME_INFO({ 143, 695, 197, 787 }, 0.1f));
	playerFrame.push_back(framework::FRAME_INFO({ 279, 698, 337, 787 }, 0.1f));
	playerFrame.push_back(framework::FRAME_INFO({ 347, 699, 406, 787 }, 0.1f));
	m_pAnimComponent->SetAnimationAsset(L"../Resource/ken.png", L"IDLE", playerFrame);

	// Move
	playerFrame.clear();
	playerFrame.push_back(framework::FRAME_INFO({ 9,883,61,965 }, 0.1f));
	playerFrame.push_back(framework::FRAME_INFO({ 71,878,130,965 }, 0.1f));
	playerFrame.push_back(framework::FRAME_INFO({ 141,877,204,966 }, 0.1f));
	playerFrame.push_back(framework::FRAME_INFO({ 216,876,278,964 }, 0.1f));
	playerFrame.push_back(framework::FRAME_INFO({ 358,878,407,966 }, 0.1f));
	m_pAnimComponent->SetAnimationAsset(L"../Resource/ken.png", L"MOVE", playerFrame);

	//m_pAnimComponent->LoadAnimationAsset(L"../Resource/ken.png", L"../Resource/TestWorld.WorldAsset");

	m_pAnimComponent->SetRelativeLocation(ScreenWidth / 2.f, ScreenHeight / 2.f);
	SetRootComponent(m_pAnimComponent);
	// Default State를 지정해주기 위한 Listener 등록
	EventManager::GetInstance()->RegisterListener(eEventType::P1SetDefaultAnimation, dynamic_cast<EventListener*>(m_pAnimComponent));
	// 이후 애니메이션 change, keep을 위한 Listener 등록
	EventManager::GetInstance()->RegisterListener(eEventType::P1KeepAnimation, dynamic_cast<EventListener*>(m_pAnimComponent));
	EventManager::GetInstance()->RegisterListener(eEventType::P1ChangeAnimation, dynamic_cast<EventListener*>(m_pAnimComponent));

	// PlayerMovement
	m_pMovementComponent = CreateComponent<PlayerMovement>(L"PlayerMovement");

	// FSMComponent
	m_pFSMComponent = CreateComponent<FSMComponent>(L"FSMComponent");
	m_pPlayerFSM = new PlayerFSM1;
	m_pFSMComponent->AttachFSM(m_pPlayerFSM);

	m_pDirectionComponent = CreateComponent<DirectionComponent>(L"DirectionComponent");
	m_pDirectionComponent->SetColor(D2D1::ColorF::Yellow);
	m_pDirectionComponent->AttachToComponent(m_pAnimComponent);

	bool res = __super::Initialize();
	assert(res);

	return true;
}

void PlayerObject0::Update(const float deltaTime)
{
	__super::Update(deltaTime);
}