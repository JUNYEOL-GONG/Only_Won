#pragma once
#include "../Engine/GameObject.h"

enum class BTN_FUNC;
class ButtonUI;
class PanelUI;

class UIObject :
    public GameObject
{
public:
    bool Initialize() final;
    void Update(const float deltaTime) final;

private:
    PanelUI* m_pStartPanel = nullptr;
    ButtonUI* m_pStartButton = nullptr;
    ButtonUI* m_pMethodButton = nullptr;
    ButtonUI* m_pMadeByButton = nullptr;
    ButtonUI* m_pExitButton = nullptr;
    ButtonUI* m_pCenterButton = nullptr;
};

