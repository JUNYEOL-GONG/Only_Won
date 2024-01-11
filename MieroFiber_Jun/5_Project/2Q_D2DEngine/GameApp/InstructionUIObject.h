#pragma once
#include "../Engine/EventListener.h"
#include "../Engine/GameObject.h"

/// <summary>
/// 
/// </summary>

class TextureComponent;
class ButtonUI;
class PanelUI;

class InstructionUIObject
	: public GameObject
	, public EventListener
{
private:
    // 게임 규칙 화면
    PanelUI* m_pInstructionWorldPanel = nullptr;
    PanelUI* m_pInstructionTitlePanel = nullptr;

    PanelUI* m_pShowPanel = nullptr;
    TextureComponent* m_pShowTexture = nullptr;

    TextureComponent* m_pTitleTexture = nullptr;
    /*TextureComponent* m_pTitleTexture = nullptr;
    TextureComponent* m_pTitleTexture = nullptr;*/

    // 표지판 대
    TextureComponent* m_pSignWoodTexture = nullptr;

    // 활성화
    TextureComponent* m_pInstructionTexture = nullptr;
    TextureComponent* m_pControllerTexture = nullptr;
    TextureComponent* m_pExitTexture = nullptr;

    // 비활성화
    TextureComponent* m_pActiveInstructionTexture = nullptr;
    TextureComponent* m_pActiveControllerTexture = nullptr;
    TextureComponent* m_pActiveExitTexture = nullptr;

    TextureComponent* m_pOkayTexture = nullptr;
    TextureComponent* m_pCancelTexture = nullptr;

    // 버튼
    ButtonUI* m_pInstructionButton = nullptr;
    ButtonUI* m_pControllerButton = nullptr;
    ButtonUI* m_pExitButton = nullptr;
    ButtonUI* m_pOkayButton = nullptr;
    ButtonUI* m_pCancelButton = nullptr;

public:
    bool Initialize() final;
    void Update(const float deltaTime) final;

    void HandleEvent(Event* event) final;
};

