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
    // ������ ������
    TextureComponent* m_pItemTexture1 = nullptr;
    TextureComponent* m_pItemTexture2 = nullptr;
    TextureComponent* m_pItemTexture3 = nullptr;
    TextureComponent* m_pItemTexture4 = nullptr;

    // �÷���Ű
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

    // �÷��̾� 1 ������ ������ �� ������
    TextureComponent* m_pPlayer1ThrowItem = nullptr;
    TextureComponent* m_pPlayer1InstallationItem = nullptr;
    TextureComponent* m_pPlayer1ReinforcedItem = nullptr;

    // �÷��̾� 2 ������ ������ �� ������
    TextureComponent* m_pPlayer2ThrowItem = nullptr;
    TextureComponent* m_pPlayer2InstallationItem = nullptr;
    TextureComponent* m_pPlayer2ReinforcedItem = nullptr;

    // �÷��̾� 3 ������ ������ �� ������
    TextureComponent* m_pPlayer3ThrowItem = nullptr;
    TextureComponent* m_pPlayer3InstallationItem = nullptr;
    TextureComponent* m_pPlayer3ReinforcedItem = nullptr;

    // �÷��̾� 4 ������ ������ �� ������
    TextureComponent* m_pPlayer4ThrowItem = nullptr;
    TextureComponent* m_pPlayer4InstallationItem = nullptr;
    TextureComponent* m_pPlayer4ReinforcedItem = nullptr;

    // ���ڱ�
    TextureComponent* m_pFootprintTexture_1 = nullptr;
    TextureComponent* m_pFootprintTexture_2 = nullptr;
    TextureComponent* m_pFootprintTexture_3 = nullptr;
    TextureComponent* m_pFootprintTexture_4 = nullptr;

    // �� �׷���
    TextureComponent* m_pGoalMoneyTexture = nullptr;

    // ��Ʈ �г�
    PanelUI* m_pInGamePanel = nullptr;

    // �׳� ��濡 �ؽ��� ���� �г�
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
