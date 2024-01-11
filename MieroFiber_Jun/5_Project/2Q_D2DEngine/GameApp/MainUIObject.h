#pragma once
#include "../Engine/GameObject.h"

/// <summary>
/// 메인 화면의 UIObject
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
    // 텍스처
    // 타이틀화면
    PanelUI* m_pMainWorldPanel = nullptr;
    PanelUI* m_pTitlePanel = nullptr;
    TextureComponent* m_pTitleTexture = nullptr;
    // 표지판 대
    TextureComponent* m_pSignWoodTexture = nullptr;

    // 비활성화
    TextureComponent* m_pGameStartTexture = nullptr;
    TextureComponent* m_pInstructionTexture = nullptr;
    TextureComponent* m_pMadeByTexture = nullptr;
    TextureComponent* m_pExitTexture = nullptr;
    TextureComponent* m_pOkayTexture = nullptr;
    TextureComponent* m_pCancelTexture = nullptr;

    // 활성화
    TextureComponent* m_pActiveGameStartTexture = nullptr;
    TextureComponent* m_pActiveInstructionTexture = nullptr;
    TextureComponent* m_pActiveMadeByTexture = nullptr;
    TextureComponent* m_pActiveExitTexture = nullptr;
    TextureComponent* m_pActiveOkayTexture = nullptr;
    TextureComponent* m_pActiveCancelTexture = nullptr;

    // 버튼
    ButtonUI* m_pStartButton = nullptr;
    ButtonUI* m_pInstructionButton = nullptr;
    ButtonUI* m_pMadeByButton = nullptr;
    ButtonUI* m_pExitButton = nullptr;
    ButtonUI* m_pOkayButton = nullptr;
    ButtonUI* m_pCancelButton = nullptr;
};