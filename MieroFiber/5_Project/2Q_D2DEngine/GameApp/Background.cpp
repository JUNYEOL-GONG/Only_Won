#include "framework.h"
#include "Background.h"

#include "InGameWorld.h"
#include "../Engine/TextureComponent.h"

Background::Background()
{
}

Background::~Background()
{
}

bool Background::Initialize()
{
	/// 땅
	m_pFloor = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pFloor->SetTextureAsset(L"../Resource/floor.png", L"Floor");
	m_pFloor->SetRelativeLocation(ScreenWidth / 2.f, ScreenHeight / 2.f);
	SetRootComponent(m_pFloor);

	m_pPebbles = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPebbles->SetTextureAsset(L"../Resource/pebbles.png", L"Pebbles");
	m_pPebbles->SetRelativeLocation(0, 0);
	m_pPebbles->AttachToComponent(m_pFloor);

	/// 절벽
	m_pLeftCliff = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pLeftCliff->SetTextureAsset(L"../Resource/cliff_left.png", L"LeftCliff");
	m_pLeftCliff->SetRelativeLocation(83.f - 50.f, 540.f);

	m_pRightCliff = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pRightCliff->SetTextureAsset(L"../Resource/cliff_right.png", L"RightCliff");
	m_pRightCliff->SetRelativeLocation(1845.f + 50.f, 540.f);

	/// 선로
	m_pTrack_1 = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pTrack_1->SetTextureAsset(L"../Resource/rail_length.png", L"TrainErack_1");
	m_pTrack_1->SetRelativeLocation(735.5f, 540.f);

	m_pTrack_2 = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pTrack_2->SetTextureAsset(L"../Resource/rail_length.png", L"TrainErack_2");
	m_pTrack_2->SetRelativeLocation(1060.5f, 540.f);

	m_pTrack_3 = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pTrack_3->SetTextureAsset(L"../Resource/rail_length.png", L"TrainErack_3");
	m_pTrack_3->SetRelativeLocation(1385.5f, 540.f);

	/// 배치물
	m_pHammer = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pHammer->SetTextureAsset(L"../Resource/hammer.png", L"Hammer");
	m_pHammer->SetRelativeLocation(899.5f, 632.5f);

	m_pPickaxe = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPickaxe->SetTextureAsset(L"../Resource/pick.png", L"Pickaxe");
	m_pPickaxe->SetRelativeLocation(515.f, 88.f);

	m_pShovel = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pShovel->SetTextureAsset(L"../Resource/shovel.png", L"Shovel");
	m_pShovel->SetRelativeLocation(1550.f, 875.f);

	m_pSkull = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pSkull->SetTextureAsset(L"../Resource/sans.png", L"Skull");
	m_pSkull->SetRelativeLocation(1210.5f, 122.f);

	m_pTruckMoney = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pTruckMoney->SetTextureAsset(L"../Resource/moneyinthehorseballoon.png", L"MoneyIcon");
	m_pTruckMoney->SetRelativeLocation(1855.f, 545.f);

	const bool res = GameObject::Initialize();
	assert(res);

	return true;
}

void Background::Update(float deltaTime)
{
	// 컴포넌트 업데이트
	GameObject::Update(deltaTime);
}