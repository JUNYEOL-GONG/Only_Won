#pragma once
#include "../Engine/GameObject.h"

/// <summary>
/// ���� ȭ���� UIObject
/// </summary>

class TextureComponent;
class ButtonUI;
class PanelUI;

class MainUIObject :
    public GameObject
{
public:
    bool Initialize() final;
    void Update(const float deltaTime) final;

private:
    // �ؽ�ó
    // Ÿ��Ʋȭ��
    PanelUI* m_pMainWorldPanel = nullptr;
    PanelUI* m_pTitlePanel = nullptr;
    TextureComponent* m_pTitleTexture = nullptr;
    // ǥ���� ��
    TextureComponent* m_pSignWoodTexture = nullptr;

    // ��Ȱ��ȭ
    TextureComponent* m_pGameStartTexture = nullptr;
    TextureComponent* m_pInstructionTexture = nullptr;
    TextureComponent* m_pMadeByTexture = nullptr;
    TextureComponent* m_pExitTexture = nullptr;
    TextureComponent* m_pOkayTexture = nullptr;
    TextureComponent* m_pCancelTexture = nullptr;

    // Ȱ��ȭ
    TextureComponent* m_pActiveGameStartTexture = nullptr;
    TextureComponent* m_pActiveInstructionTexture = nullptr;
    TextureComponent* m_pActiveMadeByTexture = nullptr;
    TextureComponent* m_pActiveExitTexture = nullptr;
    TextureComponent* m_pActiveOkayTexture = nullptr;
    TextureComponent* m_pActiveCancelTexture = nullptr;

    // ��ư
    ButtonUI* m_pStartButton = nullptr;
    ButtonUI* m_pInstructionButton = nullptr;
    ButtonUI* m_pMadeByButton = nullptr;
    ButtonUI* m_pExitButton = nullptr;
    ButtonUI* m_pOkayButton = nullptr;
    ButtonUI* m_pCancelButton = nullptr;
};