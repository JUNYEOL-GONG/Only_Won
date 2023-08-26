#pragma once
#include "../Engine/GameObject.h"

class TextureComponent;
class ButtonUI;
class PanelUI;

class MadeByUIObject :
    public GameObject
{
private:
    TextureComponent* m_pPrevTexture = nullptr;
    TextureComponent* m_pOkayTexture = nullptr;
    TextureComponent* m_pCancelTexture = nullptr;
    TextureComponent* m_pShowTexture = nullptr;
    TextureComponent* m_pBackgroundTexture = nullptr;

    PanelUI* m_pRootPanel = nullptr;
    PanelUI* m_pMadeByPanel = nullptr;
    PanelUI* m_pShowPanel = nullptr;
    ButtonUI* m_pPrevButton = nullptr;

    ButtonUI* m_pOkayButton = nullptr;
    ButtonUI* m_pCancelButton = nullptr;

public:
    bool Initialize() final;
    void Update(const float deltaTime) final;
};

