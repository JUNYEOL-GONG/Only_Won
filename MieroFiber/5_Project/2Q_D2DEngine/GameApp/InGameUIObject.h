#pragma once

#include "../Engine/EventListener.h"
#include "../Engine/GameObject.h"

class TextureComponent;
class ButtonUI;
class PanelUI;

class InGameUIObject
	: public GameObject
	, public EventListener
{
private:
    // 아이템 보관함
    TextureComponent* m_pItemTexture1 = nullptr;
    TextureComponent* m_pItemTexture2 = nullptr;
    TextureComponent* m_pItemTexture3 = nullptr;
    TextureComponent* m_pItemTexture4 = nullptr;

    // 플레이키
    TextureComponent* m_pPlayer1RT = nullptr;
    TextureComponent* m_pPlayer1X = nullptr;
    TextureComponent* m_pPlayer1A = nullptr;
    TextureComponent* m_pPlayer1B = nullptr;

    TextureComponent* m_pPlayer2RT = nullptr;
    TextureComponent* m_pPlayer2X = nullptr;
    TextureComponent* m_pPlayer2A = nullptr;
    TextureComponent* m_pPlayer2B = nullptr;

    TextureComponent* m_pPlayer3RT = nullptr;
    TextureComponent* m_pPlayer3X = nullptr;
    TextureComponent* m_pPlayer3A = nullptr;
    TextureComponent* m_pPlayer3B = nullptr;

    TextureComponent* m_pPlayer4RT = nullptr;
    TextureComponent* m_pPlayer4X = nullptr;
    TextureComponent* m_pPlayer4A = nullptr;
    TextureComponent* m_pPlayer4B = nullptr;

    // 플레이어 1 아이템 보관함 속 아이템
    TextureComponent* m_pPlayer1ThrowItem = nullptr;
    TextureComponent* m_pPlayer1InstallationItem = nullptr;
    TextureComponent* m_pPlayer1ReinforcedItem = nullptr;

    // 플레이어 2 아이템 보관함 속 아이템
    TextureComponent* m_pPlayer2ThrowItem = nullptr;
    TextureComponent* m_pPlayer2InstallationItem = nullptr;
    TextureComponent* m_pPlayer2ReinforcedItem = nullptr;

    // 플레이어 3 아이템 보관함 속 아이템
    TextureComponent* m_pPlayer3ThrowItem = nullptr;
    TextureComponent* m_pPlayer3InstallationItem = nullptr;
    TextureComponent* m_pPlayer3ReinforcedItem = nullptr;

    // 플레이어 4 아이템 보관함 속 아이템
    TextureComponent* m_pPlayer4ThrowItem = nullptr;
    TextureComponent* m_pPlayer4InstallationItem = nullptr;
    TextureComponent* m_pPlayer4ReinforcedItem = nullptr;

    // 발자국
    TextureComponent* m_pFootprintTexture_1 = nullptr;
    TextureComponent* m_pFootprintTexture_2 = nullptr;
    TextureComponent* m_pFootprintTexture_3 = nullptr;
    TextureComponent* m_pFootprintTexture_4 = nullptr;

    // 돈 그래프
    TextureComponent* m_pGoalMoneyTexture = nullptr;

    // 루트 패널
    PanelUI* m_pInGamePanel = nullptr;

    // 그냥 배경에 텍스쳐 붙일 패널
    PanelUI* m_pTexturePanel = nullptr;

    PanelUI* m_pGoalMoneyPanel = nullptr;

    PanelUI* m_Player1Item = nullptr;
    //PanelUI* m_Player1Item2 = nullptr;
    //PanelUI* m_Player1Item3 = nullptr;

    PanelUI* m_Player2Item = nullptr;
    //PanelUI* m_Player2Item2 = nullptr;
    //PanelUI* m_Player2Item3 = nullptr;

    PanelUI* m_Player3Item = nullptr;
    //PanelUI* m_Player3Item2 = nullptr;
    //PanelUI* m_Player3Item3 = nullptr;

    PanelUI* m_Player4Item = nullptr;
    //PanelUI* m_Player4Item2 = nullptr;
    //PanelUI* m_Player4Item3 = nullptr;

public:
    bool Initialize() final;
    void Update(const float deltaTime) final;

    void HandleEvent(Event* event) final;
};
