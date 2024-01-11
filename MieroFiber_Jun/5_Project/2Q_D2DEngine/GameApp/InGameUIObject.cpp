#include "framework.h"
#include "InGameUIObject.h"

#include "ItemBoxObject.h"
#include "ItemObject.h"
#include "../Engine/ButtonUI.h"
#include "../Engine/PanelUI.h"
#include "../Engine/EventManager.h"
#include "../Engine/TextureComponent.h"

bool InGameUIObject::Initialize()
{
	EventManager::GetInstance()->RegisterListener(eEventType::ItemVisible, this);
	EventManager::GetInstance()->RegisterListener(eEventType::ItemInVisible, this);
	EventManager::GetInstance()->RegisterListener(eEventType::FootprintVisible, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::FootprintInVisible, dynamic_cast<EventListener*>(this));

	// UI 배치
	m_pInGamePanel = CreateComponent<PanelUI>(L"InGamePanel");
	m_pInGamePanel->SetWidth(0);
	m_pInGamePanel->SetHeight(0);
	m_pInGamePanel->SetColor(D2D1::ColorF::White);
	m_pInGamePanel->SetRelativeLocation(0.f, 0.f);
	SetRootComponent(m_pInGamePanel);

	m_pTexturePanel = CreateComponent<PanelUI>(L"m_pTexturePanel");
	m_pTexturePanel->SetWidth(0);
	m_pTexturePanel->SetHeight(0);
	m_pTexturePanel->SetColor(D2D1::ColorF::White);
	m_pTexturePanel->SetRelativeLocation(0, 0);
	m_pTexturePanel->SetActiveUI(false);
	m_pTexturePanel->AttachToComponent(m_pInGamePanel);

	m_pFootprintTexture_1 = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pFootprintTexture_1->SetTextureAsset(L"../Resource/투명화발자국.png", L"GameStartTexture");
	m_pFootprintTexture_1->SetRelativeLocation(0.f, 0.f);
	m_pFootprintTexture_1->SetShowTexture(false);
	m_pFootprintTexture_1->AttachToComponent(m_pTexturePanel);

	m_pFootprintTexture_2 = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pFootprintTexture_2->SetTextureAsset(L"../Resource/투명화발자국.png", L"GameStartTexture");
	m_pFootprintTexture_2->SetRelativeLocation(0.f, 0.f);
	m_pFootprintTexture_2->SetShowTexture(false);
	m_pFootprintTexture_2->AttachToComponent(m_pTexturePanel);

	m_pFootprintTexture_3 = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pFootprintTexture_3->SetTextureAsset(L"../Resource/투명화발자국.png", L"GameStartTexture");
	m_pFootprintTexture_3->SetRelativeLocation(0.f, 0.f);
	m_pFootprintTexture_3->SetShowTexture(false);
	m_pFootprintTexture_3->AttachToComponent(m_pTexturePanel);

	m_pFootprintTexture_4 = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pFootprintTexture_4->SetTextureAsset(L"../Resource/투명화발자국.png", L"GameStartTexture");
	m_pFootprintTexture_4->SetRelativeLocation(0.f, 0.f);
	m_pFootprintTexture_4->SetShowTexture(false);
	m_pFootprintTexture_4->AttachToComponent(m_pTexturePanel);

	m_pGoalMoneyPanel = CreateComponent<PanelUI>(L"GoalMoneyPanel");
	m_pGoalMoneyPanel->SetWidth(1100.f);
	m_pGoalMoneyPanel->SetHeight(30.f);
	m_pGoalMoneyPanel->SetColor(D2D1::ColorF::Red);
	m_pGoalMoneyPanel->SetRelativeLocation(960.f, 65.f);
	m_pGoalMoneyPanel->SetActiveUI(false);
	m_pGoalMoneyPanel->AttachToComponent(m_pInGamePanel);

	m_pGoalMoneyTexture = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pGoalMoneyTexture->SetTextureAsset(L"../Resource/graph.png", L"GameStartTexture");
	m_pGoalMoneyTexture->SetRelativeLocation(0.f, 0.f);
	m_pGoalMoneyTexture->AttachToComponent(m_pGoalMoneyPanel);

	m_Player1Item = CreateComponent<PanelUI>(L"Player1Item");
	m_Player1Item->SetWidth(350.f);
	m_Player1Item->SetHeight(140.f);
	m_Player1Item->SetColor(D2D1::ColorF::Red);
	m_Player1Item->SetRelativeLocation(185.f, 80.f);
	m_Player1Item->SetActiveUI(false);
	m_Player1Item->AttachToComponent(m_pInGamePanel);

	m_Player2Item = CreateComponent<PanelUI>(L"Player2Item");
	m_Player2Item->SetWidth(350.f);
	m_Player2Item->SetHeight(140.f);
	m_Player2Item->SetColor(D2D1::ColorF::Red);
	m_Player2Item->SetRelativeLocation(1735.f, 80.f);
	m_Player2Item->SetActiveUI(false);
	m_Player2Item->AttachToComponent(m_pInGamePanel);

	m_Player3Item = CreateComponent<PanelUI>(L"Player3Item");
	m_Player3Item->SetWidth(350.f);
	m_Player3Item->SetHeight(140.f);
	m_Player3Item->SetColor(D2D1::ColorF::Red);
	m_Player3Item->SetRelativeLocation(185.f, 1000.f);
	m_Player3Item->SetActiveUI(false);
	m_Player3Item->AttachToComponent(m_pInGamePanel);

	m_Player4Item = CreateComponent<PanelUI>(L"Player4Item");
	m_Player4Item->SetWidth(350.f);
	m_Player4Item->SetHeight(120.f);
	m_Player4Item->SetColor(D2D1::ColorF::Red);
	m_Player4Item->SetRelativeLocation(1735.f, 1000.f);
	m_Player4Item->SetActiveUI(false);
	m_Player4Item->AttachToComponent(m_pInGamePanel);

	m_pItemTexture1 = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pItemTexture1->SetTextureAsset(L"../Resource/Inventory_red.png", L"GameStartTexture");
	m_pItemTexture1->SetRelativeLocation(0.f, 0.f);
	m_pItemTexture1->AttachToComponent(m_Player1Item);

	m_pPlayer1ThrowItem = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer1ThrowItem->SetTextureAsset(L"../Resource/snowBall.png", L"GameStartTexture");
	m_pPlayer1ThrowItem->SetRelativeLocation(78.f, 80.f);
	m_pPlayer1ThrowItem->SetShowTexture(false);
	m_pPlayer1ThrowItem->AttachToComponent(m_pTexturePanel);

	m_pPlayer1InstallationItem = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer1InstallationItem->SetTextureAsset(L"../Resource/snowBall.png", L"GameStartTexture");
	m_pPlayer1InstallationItem->SetRelativeLocation(186.f, 80.f);
	m_pPlayer1InstallationItem->SetShowTexture(false);
	m_pPlayer1InstallationItem->AttachToComponent(m_pTexturePanel);

	m_pPlayer1ReinforcedItem = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer1ReinforcedItem->SetTextureAsset(L"../Resource/snowBall.png", L"GameStartTexture");
	m_pPlayer1ReinforcedItem->SetRelativeLocation(297.f, 80.f);
	m_pPlayer1ReinforcedItem->SetShowTexture(false);
	m_pPlayer1ReinforcedItem->AttachToComponent(m_pTexturePanel);

	m_pPlayer1RT = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer1RT->SetTextureAsset(L"../Resource/button_RT.png", L"GameStartTexture");
	m_pPlayer1RT->SetRelativeLocation(42.5f, 35.5f);
	m_pPlayer1RT->AttachToComponent(m_pTexturePanel);

	m_pPlayer1X = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer1X->SetTextureAsset(L"../Resource/button_X.png", L"GameStartTexture");
	m_pPlayer1X->SetRelativeLocation(112.5f, 122.5f);
	m_pPlayer1X->AttachToComponent(m_pTexturePanel);

	m_pPlayer1A = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer1A->SetTextureAsset(L"../Resource/button_A.png", L"GameStartTexture");
	m_pPlayer1A->SetRelativeLocation(222.5f, 122.5f);
	m_pPlayer1A->AttachToComponent(m_pTexturePanel);

	m_pPlayer1B = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer1B->SetTextureAsset(L"../Resource/button_B.png", L"GameStartTexture");
	m_pPlayer1B->SetRelativeLocation(332.5f, 122.5f);
	m_pPlayer1B->AttachToComponent(m_pTexturePanel); 

	m_pItemTexture2 = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pItemTexture2->SetTextureAsset(L"../Resource/Inventory_blue.png", L"GameStartTexture");
	m_pItemTexture2->SetRelativeLocation(0.f, 0.f);
	m_pItemTexture2->AttachToComponent(m_Player2Item);

	m_pPlayer2ThrowItem = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer2ThrowItem->SetTextureAsset(L"../Resource/snowBall.png", L"GameStartTexture");
	m_pPlayer2ThrowItem->SetRelativeLocation(1628.f, 80.f);
	m_pPlayer2ThrowItem->SetShowTexture(false);
	m_pPlayer2ThrowItem->AttachToComponent(m_pTexturePanel);

	m_pPlayer2InstallationItem = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer2InstallationItem->SetTextureAsset(L"../Resource/snowBall.png", L"GameStartTexture");
	m_pPlayer2InstallationItem->SetRelativeLocation(1736.f, 80.f);
	m_pPlayer2InstallationItem->SetShowTexture(false);
	m_pPlayer2InstallationItem->AttachToComponent(m_pTexturePanel);

	m_pPlayer2ReinforcedItem = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer2ReinforcedItem->SetTextureAsset(L"../Resource/snowBall.png", L"GameStartTexture");
	m_pPlayer2ReinforcedItem->SetRelativeLocation(1848.f, 80.f);
	m_pPlayer2ReinforcedItem->SetShowTexture(false);
	m_pPlayer2ReinforcedItem->AttachToComponent(m_pTexturePanel);

	m_pPlayer2RT = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer2RT->SetTextureAsset(L"../Resource/button_RT.png", L"GameStartTexture");
	m_pPlayer2RT->SetRelativeLocation(1592.5f, 35.5f);
	m_pPlayer2RT->AttachToComponent(m_pTexturePanel);
			 
	m_pPlayer2X = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer2X->SetTextureAsset(L"../Resource/button_X.png", L"GameStartTexture");
	m_pPlayer2X->SetRelativeLocation(1662.5f, 122.5f);
	m_pPlayer2X->AttachToComponent(m_pTexturePanel);
			 
	m_pPlayer2A = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer2A->SetTextureAsset(L"../Resource/button_A.png", L"GameStartTexture");
	m_pPlayer2A->SetRelativeLocation(1772.5f, 122.5f);
	m_pPlayer2A->AttachToComponent(m_pTexturePanel);
			 
	m_pPlayer2B = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer2B->SetTextureAsset(L"../Resource/button_B.png", L"GameStartTexture");
	m_pPlayer2B->SetRelativeLocation(1882.5f, 122.5f);
	m_pPlayer2B->AttachToComponent(m_pTexturePanel);

	m_pItemTexture3 = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pItemTexture3->SetTextureAsset(L"../Resource/Inventory_yellow.png", L"GameStartTexture");
	m_pItemTexture3->SetRelativeLocation(0.f, 0.f);
	m_pItemTexture3->AttachToComponent(m_Player3Item);

	m_pPlayer3ThrowItem = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer3ThrowItem->SetTextureAsset(L"../Resource/snowBall.png", L"GameStartTexture");
	m_pPlayer3ThrowItem->SetRelativeLocation(78.f, 1000.f);
	m_pPlayer3ThrowItem->SetShowTexture(false);
	m_pPlayer3ThrowItem->AttachToComponent(m_pTexturePanel);

	m_pPlayer3InstallationItem = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer3InstallationItem->SetTextureAsset(L"../Resource/snowBall.png", L"GameStartTexture");
	m_pPlayer3InstallationItem->SetRelativeLocation(186.f, 1000.f);
	m_pPlayer3InstallationItem->SetShowTexture(false);
	m_pPlayer3InstallationItem->AttachToComponent(m_pTexturePanel);

	m_pPlayer3ReinforcedItem = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer3ReinforcedItem->SetTextureAsset(L"../Resource/snowBall.png", L"GameStartTexture");
	m_pPlayer3ReinforcedItem->SetRelativeLocation(297.f, 1000.f);
	m_pPlayer3ReinforcedItem->SetShowTexture(false);
	m_pPlayer3ReinforcedItem->AttachToComponent(m_pTexturePanel);

	m_pPlayer3RT = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer3RT->SetTextureAsset(L"../Resource/button_RT.png", L"GameStartTexture");
	m_pPlayer3RT->SetRelativeLocation(42.5f, 955.5f);
	m_pPlayer3RT->AttachToComponent(m_pTexturePanel);
			 
	m_pPlayer3X = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer3X->SetTextureAsset(L"../Resource/button_X.png", L"GameStartTexture");
	m_pPlayer3X->SetRelativeLocation(122.5f, 1045.5f);
	m_pPlayer3X->AttachToComponent(m_pTexturePanel);
			 
	m_pPlayer3A = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer3A->SetTextureAsset(L"../Resource/button_A.png", L"GameStartTexture");
	m_pPlayer3A->SetRelativeLocation(222.5f, 1045.5f);
	m_pPlayer3A->AttachToComponent(m_pTexturePanel);
			 
	m_pPlayer3B = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer3B->SetTextureAsset(L"../Resource/button_B.png", L"GameStartTexture");
	m_pPlayer3B->SetRelativeLocation(332.5f, 1045.5f);
	m_pPlayer3B->AttachToComponent(m_pTexturePanel);

	m_pItemTexture4 = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pItemTexture4->SetTextureAsset(L"../Resource/Inventory_green.png", L"GameStartTexture");
	m_pItemTexture4->SetRelativeLocation(0.f, 0.f);
	m_pItemTexture4->AttachToComponent(m_Player4Item);

	m_pPlayer4ThrowItem = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer4ThrowItem->SetTextureAsset(L"../Resource/snowBall.png", L"GameStartTexture");
	m_pPlayer4ThrowItem->SetRelativeLocation(1628.f, 1000.f);
	m_pPlayer4ThrowItem->SetShowTexture(false);
	m_pPlayer4ThrowItem->AttachToComponent(m_pTexturePanel);

	m_pPlayer4InstallationItem = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer4InstallationItem->SetTextureAsset(L"../Resource/snowBall.png", L"GameStartTexture");
	m_pPlayer4InstallationItem->SetRelativeLocation(1736.f, 1000.f);
	m_pPlayer4InstallationItem->SetShowTexture(false);
	m_pPlayer4InstallationItem->AttachToComponent(m_pTexturePanel);

	m_pPlayer4ReinforcedItem = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer4ReinforcedItem->SetTextureAsset(L"../Resource/snowBall.png", L"GameStartTexture");
	m_pPlayer4ReinforcedItem->SetRelativeLocation(1848.f, 1000.f);
	m_pPlayer4ReinforcedItem->SetShowTexture(false);
	m_pPlayer4ReinforcedItem->AttachToComponent(m_pTexturePanel); 

	m_pPlayer4RT = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer4RT->SetTextureAsset(L"../Resource/button_RT.png", L"GameStartTexture");
	m_pPlayer4RT->SetRelativeLocation(1592.5f, 955.5f);
	m_pPlayer4RT->AttachToComponent(m_pTexturePanel);
			 
	m_pPlayer4X = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer4X->SetTextureAsset(L"../Resource/button_X.png", L"GameStartTexture");
	m_pPlayer4X->SetRelativeLocation(1662.5f, 1045.5f);
	m_pPlayer4X->AttachToComponent(m_pTexturePanel);
			 
	m_pPlayer4A = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer4A->SetTextureAsset(L"../Resource/button_A.png", L"GameStartTexture");
	m_pPlayer4A->SetRelativeLocation(1772.5f, 1045.5f);
	m_pPlayer4A->AttachToComponent(m_pTexturePanel);
			 
	m_pPlayer4B = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pPlayer4B->SetTextureAsset(L"../Resource/button_B.png", L"GameStartTexture");
	m_pPlayer4B->SetRelativeLocation(1882.5f, 1045.5f);
	m_pPlayer4B->AttachToComponent(m_pTexturePanel);

	__super::Initialize();

	return true;
}

void InGameUIObject::Update(const float deltaTime)
{
	// 여기
	__super::Update(deltaTime);
}

void InGameUIObject::HandleEvent(Event* event)
{
	// 플레이어들이 아이템을 먹었을 때
	if(event->eventID == eEventType::ItemVisible)
	{
		// 플레이어 1
		if (event->playerNum == 0)
		{
			if (event->itemBoxType == eItemBoxType::THROW)
			{
				if (event->itemType == eItemType::PunchItem)
				{
					m_pPlayer1ThrowItem->SetTextureAsset(L"../Resource/펀치.png", L"GameStartTexture");
					m_pPlayer1ThrowItem->SetShowTexture(true);
				}
				else if (event->itemType == eItemType::SnowItem)
				{
					m_pPlayer1ThrowItem->SetTextureAsset(L"../Resource/눈덩이.png", L"GameStartTexture");
					m_pPlayer1ThrowItem->SetShowTexture(true);
				}
				else if (event->itemType == eItemType::StungunItem)
				{
					m_pPlayer1ThrowItem->SetTextureAsset(L"../Resource/Icon_Stun_gun.png", L"GameStartTexture");
					m_pPlayer1ThrowItem->SetShowTexture(true);
				}
				else if (event->itemType == eItemType::BottleItem)
				{
					m_pPlayer1ThrowItem->SetTextureAsset(L"../Resource/유리병.png", L"GameStartTexture");
					m_pPlayer1ThrowItem->SetShowTexture(true);
				}
			}
			else if (event->itemBoxType == eItemBoxType::INSTALLATION)
			{
				if (event->itemType == eItemType::SnareItem)
				{
					m_pPlayer1InstallationItem->SetTextureAsset(L"../Resource/덫.png", L"GameStartTexture");
					m_pPlayer1InstallationItem->SetShowTexture(true);
				}
				if (event->itemType == eItemType::TrapItem)
				{
					m_pPlayer1InstallationItem->SetTextureAsset(L"../Resource/icon_hole.png", L"GameStartTexture");
					m_pPlayer1InstallationItem->SetShowTexture(true);
				}
			}
			else if (event->itemBoxType == eItemBoxType::REINFORCED)
			{
				if (event->itemType == eItemType::ShoeItem)
				{
					m_pPlayer1ReinforcedItem->SetTextureAsset(L"../Resource/신발아이콘.png", L"GameStartTexture");
					m_pPlayer1ReinforcedItem->SetShowTexture(true);
				}

				else if (event->itemType == eItemType::WaveItem)
				{
					m_pPlayer1ReinforcedItem->SetTextureAsset(L"../Resource/해일.png", L"GameStartTexture");
					m_pPlayer1ReinforcedItem->SetShowTexture(true);
				}
				else if (event->itemType == eItemType::TransparencyItem)
				{
					m_pPlayer1ReinforcedItem->SetTextureAsset(L"../Resource/투명화.png", L"GameStartTexture");
					m_pPlayer1ReinforcedItem->SetShowTexture(true);
				}
			}
		}
		// 플레이어 2
		else if (event->playerNum == 1)
		{
			if (event->itemBoxType == eItemBoxType::THROW)
			{
				if (event->itemType == eItemType::PunchItem)
				{
					m_pPlayer2ThrowItem->SetTextureAsset(L"../Resource/펀치.png", L"GameStartTexture");
					m_pPlayer2ThrowItem->SetShowTexture(true);
				}
				else if (event->itemType == eItemType::SnowItem)
				{
					m_pPlayer2ThrowItem->SetTextureAsset(L"../Resource/눈덩이.png", L"GameStartTexture");
					m_pPlayer2ThrowItem->SetShowTexture(true);
				}
				else if (event->itemType == eItemType::StungunItem)
				{
					m_pPlayer2ThrowItem->SetTextureAsset(L"../Resource/Icon_Stun_gun.png", L"GameStartTexture");
					m_pPlayer2ThrowItem->SetShowTexture(true);
				}
				else if (event->itemType == eItemType::BottleItem)
				{
					m_pPlayer2ThrowItem->SetTextureAsset(L"../Resource/유리병.png", L"GameStartTexture");
					m_pPlayer2ThrowItem->SetShowTexture(true);
				}
			}
			else if (event->itemBoxType == eItemBoxType::INSTALLATION)
			{
				if (event->itemType == eItemType::SnareItem)
				{
					m_pPlayer2InstallationItem->SetTextureAsset(L"../Resource/덫.png", L"GameStartTexture");
					m_pPlayer2InstallationItem->SetShowTexture(true);
				}
				if (event->itemType == eItemType::TrapItem)
				{
					m_pPlayer2InstallationItem->SetTextureAsset(L"../Resource/icon_hole.png", L"GameStartTexture");
					m_pPlayer2InstallationItem->SetShowTexture(true);
				}
			}
			else if (event->itemBoxType == eItemBoxType::REINFORCED)
			{
				if (event->itemType == eItemType::ShoeItem)
				{
					m_pPlayer2ReinforcedItem->SetTextureAsset(L"../Resource/신발아이콘.png", L"GameStartTexture");
					m_pPlayer2ReinforcedItem->SetShowTexture(true);
				}
				else if (event->itemType == eItemType::WaveItem)
				{
					m_pPlayer2ReinforcedItem->SetTextureAsset(L"../Resource/해일.png", L"GameStartTexture");
					m_pPlayer2ReinforcedItem->SetShowTexture(true);
				}
				else if (event->itemType == eItemType::TransparencyItem)
				{
					m_pPlayer2ReinforcedItem->SetTextureAsset(L"../Resource/투명화.png", L"GameStartTexture");
					m_pPlayer2ReinforcedItem->SetShowTexture(true);
				}
			}
		}

		// 플레이어 3
		else if (event->playerNum == 2)
		{
			if (event->itemBoxType == eItemBoxType::THROW)
			{
				if (event->itemType == eItemType::PunchItem)
				{
					m_pPlayer3ThrowItem->SetTextureAsset(L"../Resource/펀치.png", L"GameStartTexture");
					m_pPlayer3ThrowItem->SetShowTexture(true);
				}
				else if (event->itemType == eItemType::SnowItem)
				{
					m_pPlayer3ThrowItem->SetTextureAsset(L"../Resource/눈덩이.png", L"GameStartTexture");
					m_pPlayer3ThrowItem->SetShowTexture(true);
				}
				else if (event->itemType == eItemType::StungunItem)
				{
					m_pPlayer3ThrowItem->SetTextureAsset(L"../Resource/Icon_Stun_gun.png", L"GameStartTexture");
					m_pPlayer3ThrowItem->SetShowTexture(true);
				}
				else if (event->itemType == eItemType::BottleItem)
				{
					m_pPlayer3ThrowItem->SetTextureAsset(L"../Resource/유리병.png", L"GameStartTexture");
					m_pPlayer3ThrowItem->SetShowTexture(true);
				}
			}
			else if (event->itemBoxType == eItemBoxType::INSTALLATION)
			{
				if (event->itemType == eItemType::SnareItem)
				{
					m_pPlayer3InstallationItem->SetTextureAsset(L"../Resource/덫.png", L"GameStartTexture");
					m_pPlayer3InstallationItem->SetShowTexture(true);
				}
				if (event->itemType == eItemType::TrapItem)
				{
					m_pPlayer3InstallationItem->SetTextureAsset(L"../Resource/icon_hole.png", L"GameStartTexture");
					m_pPlayer3InstallationItem->SetShowTexture(true);
				}
			}
			else if (event->itemBoxType == eItemBoxType::REINFORCED)
			{
				if (event->itemType == eItemType::ShoeItem)
				{
					m_pPlayer3ReinforcedItem->SetTextureAsset(L"../Resource/신발아이콘.png", L"GameStartTexture");
					m_pPlayer3ReinforcedItem->SetShowTexture(true);
				}
				else if (event->itemType == eItemType::WaveItem)
				{
					m_pPlayer3ReinforcedItem->SetTextureAsset(L"../Resource/해일.png", L"GameStartTexture");
					m_pPlayer3ReinforcedItem->SetShowTexture(true);
				}
				else if (event->itemType == eItemType::TransparencyItem)
				{
					m_pPlayer3ReinforcedItem->SetTextureAsset(L"../Resource/투명화.png", L"GameStartTexture");
					m_pPlayer3ReinforcedItem->SetShowTexture(true);
				}
			}
		}
		// 플레이어 4
		else if (event->playerNum == 3)
		{
			if (event->itemBoxType == eItemBoxType::THROW)
			{
				if (event->itemType == eItemType::PunchItem)
				{
					m_pPlayer4ThrowItem->SetTextureAsset(L"../Resource/펀치.png", L"GameStartTexture");
					m_pPlayer4ThrowItem->SetShowTexture(true);
				}
				else if (event->itemType == eItemType::SnowItem)
				{
					m_pPlayer4ThrowItem->SetTextureAsset(L"../Resource/눈덩이.png", L"GameStartTexture");
					m_pPlayer4ThrowItem->SetShowTexture(true);
				}
				else if (event->itemType == eItemType::StungunItem)
				{
					m_pPlayer4ThrowItem->SetTextureAsset(L"../Resource/Icon_Stun_gun.png", L"GameStartTexture");
					m_pPlayer4ThrowItem->SetShowTexture(true);
				}
				else if (event->itemType == eItemType::BottleItem)
				{
					m_pPlayer4ThrowItem->SetTextureAsset(L"../Resource/유리병.png", L"GameStartTexture");
					m_pPlayer4ThrowItem->SetShowTexture(true);
				}
			}
			else if (event->itemBoxType == eItemBoxType::INSTALLATION)
			{
				if (event->itemType == eItemType::SnareItem)
				{
					m_pPlayer4InstallationItem->SetTextureAsset(L"../Resource/덫.png", L"GameStartTexture");
					m_pPlayer4InstallationItem->SetShowTexture(true);
				}
				if (event->itemType == eItemType::TrapItem)
				{
					m_pPlayer4InstallationItem->SetTextureAsset(L"../Resource/icon_hole.png", L"GameStartTexture");
					m_pPlayer4InstallationItem->SetShowTexture(true);
				}
			}
			else if (event->itemBoxType == eItemBoxType::REINFORCED)
			{
				if (event->itemType == eItemType::ShoeItem)
				{
					m_pPlayer4ReinforcedItem->SetTextureAsset(L"../Resource/신발아이콘.png", L"GameStartTexture");
					m_pPlayer4ReinforcedItem->SetShowTexture(true);
				}
				else if (event->itemType == eItemType::WaveItem)
				{
					m_pPlayer4ReinforcedItem->SetTextureAsset(L"../Resource/해일.png", L"GameStartTexture");
					m_pPlayer4ReinforcedItem->SetShowTexture(true);
				}
				else if (event->itemType == eItemType::TransparencyItem)
				{
					m_pPlayer4ReinforcedItem->SetTextureAsset(L"../Resource/투명화.png", L"GameStartTexture");
					m_pPlayer4ReinforcedItem->SetShowTexture(true);
				}
			}
		}
	}
	// 플레이어들이 아이템 사용했을 때
	else if(event->eventID == eEventType::ItemInVisible)
	{
		// 플레이어 1
		if (event->playerNum == 0)
		{
			if (event->itemBoxType == eItemBoxType::THROW)
			{
				m_pPlayer1ThrowItem->SetShowTexture(false);
			}
			else if (event->itemBoxType == eItemBoxType::INSTALLATION)
			{
				m_pPlayer1InstallationItem->SetShowTexture(false);
			}
			else if (event->itemBoxType == eItemBoxType::REINFORCED)
			{
				m_pPlayer1ReinforcedItem->SetShowTexture(false);
			}
		}
		// 플레이어 2
		else if (event->playerNum == 1)
		{
			if (event->itemBoxType == eItemBoxType::THROW)
			{
				m_pPlayer2ThrowItem->SetShowTexture(false);
			}
			else if (event->itemBoxType == eItemBoxType::INSTALLATION)
			{
				m_pPlayer2InstallationItem->SetShowTexture(false);
			}
			else if (event->itemBoxType == eItemBoxType::REINFORCED)
			{
				m_pPlayer2ReinforcedItem->SetShowTexture(false);
			}
		}
		// 플레이어 3
		else if (event->playerNum == 2)
		{
			if (event->itemBoxType == eItemBoxType::THROW)
			{
				m_pPlayer3ThrowItem->SetShowTexture(false);
			}
			else if (event->itemBoxType == eItemBoxType::INSTALLATION)
			{
				m_pPlayer3InstallationItem->SetShowTexture(false);
			}
			else if (event->itemBoxType == eItemBoxType::REINFORCED)
			{
				m_pPlayer3ReinforcedItem->SetShowTexture(false);
			}
		}
		// 플레이어 4
		else if (event->playerNum == 3)
		{
			if (event->itemBoxType == eItemBoxType::THROW)
			{
				m_pPlayer4ThrowItem->SetShowTexture(false);
			}
			else if (event->itemBoxType == eItemBoxType::INSTALLATION)
			{
				m_pPlayer4InstallationItem->SetShowTexture(false);
			}
			else if (event->itemBoxType == eItemBoxType::REINFORCED)
			{
				m_pPlayer4ReinforcedItem->SetShowTexture(false);
			}
		}
	}

	// 발바닥 보여주기 시작
	if (event->eventID == eEventType::FootprintVisible)
	{
		if (event->playerNum == 0)
		{
			// 플레이어 1
			m_pFootprintTexture_1->SetRelativeLocation(event->playerLocation.x, event->playerLocation.y);
			float theta = atan(event->playerLookDirection.y / event->playerLookDirection.x);
			theta = theta * 180.f / 3.14159265f;
			m_pFootprintTexture_1->SetRelativeRotation(theta);

			// 투사체 아이템의 이미지 반전
			if (event->playerLookDirection.x < 0)
				m_pFootprintTexture_1->SetFlipX(true);
			else if (event->playerLookDirection.x >= 0)
				m_pFootprintTexture_1->SetFlipX(false);

			m_pFootprintTexture_1->SetShowTexture(true);
		}
		else if (event->playerNum == 1)
		{
			// 플레이어 2
			m_pFootprintTexture_2->SetRelativeLocation(event->playerLocation.x, event->playerLocation.y);
			float theta = atan(event->playerLookDirection.y / event->playerLookDirection.x);
			theta = theta * 180.f / 3.14159265f;
			m_pFootprintTexture_2->SetRelativeRotation(theta);

			// 투사체 아이템의 이미지 반전
			if (event->playerLookDirection.x < 0)
				m_pFootprintTexture_2->SetFlipX(true);
			else if (event->playerLookDirection.x >= 0)
				m_pFootprintTexture_2->SetFlipX(false);
			m_pFootprintTexture_2->SetShowTexture(true);
		}
		else if (event->playerNum == 2)
		{
			// 플레이어 3
			m_pFootprintTexture_3->SetRelativeLocation(event->playerLocation.x, event->playerLocation.y);
			float theta = atan(event->playerLookDirection.y / event->playerLookDirection.x);
			theta = theta * 180.f / 3.14159265f;
			m_pFootprintTexture_3->SetRelativeRotation(theta);

			// 투사체 아이템의 이미지 반전
			if (event->playerLookDirection.x < 0)
				m_pFootprintTexture_3->SetFlipX(true);
			else if (event->playerLookDirection.x >= 0)
				m_pFootprintTexture_3->SetFlipX(false);
			m_pFootprintTexture_3->SetShowTexture(true);
		}
		else if (event->playerNum == 3)
		{
			// 플레이어 4
			m_pFootprintTexture_4->SetRelativeLocation(event->playerLocation.x, event->playerLocation.y);
			float theta = atan(event->playerLookDirection.y / event->playerLookDirection.x);
			theta = theta * 180.f / 3.14159265f;
			m_pFootprintTexture_4->SetRelativeRotation(theta);

			// 투사체 아이템의 이미지 반전
			if (event->playerLookDirection.x < 0)
				m_pFootprintTexture_4->SetFlipX(true);
			else if (event->playerLookDirection.x >= 0)
				m_pFootprintTexture_4->SetFlipX(false);
			m_pFootprintTexture_4->SetShowTexture(true);
		}

		return;
	}
	// 발바닥 보여주기 끝
	if (event->eventID == eEventType::FootprintInVisible)
	{
		if (event->playerNum == 0)
		{
			// 플레이어 1
			m_pFootprintTexture_1->SetRelativeLocation(0.f, 0.f);
			m_pFootprintTexture_1->SetShowTexture(false);
		}
		else if (event->playerNum == 1)
		{
			// 플레이어 2
			m_pFootprintTexture_2->SetRelativeLocation(0.f, 0.f);
			m_pFootprintTexture_2->SetShowTexture(false);
		}
		else if (event->playerNum == 2)
		{
			// 플레이어 3
			m_pFootprintTexture_3->SetRelativeLocation(0.f, 0.f);
			m_pFootprintTexture_3->SetShowTexture(false);
		}
		else if (event->playerNum == 3)
		{
			// 플레이어 4
			m_pFootprintTexture_4->SetRelativeLocation(0.f, 0.f);
			m_pFootprintTexture_4->SetShowTexture(false);
		}
		return;
	}
}
