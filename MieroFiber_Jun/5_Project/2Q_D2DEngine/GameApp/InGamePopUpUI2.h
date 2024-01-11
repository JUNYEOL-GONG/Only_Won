#pragma once
#include "../Engine/EventListener.h"
#include "../Engine/GameObject.h"

class TextureComponent;
class PanelUI;

class InGamePopUpUI2 :
    public GameObject
{
public:
    bool Initialize() final;
    void Update(const float deltaTime) final;

private:
    PanelUI* m_pRootPanel = nullptr;
    PanelUI* m_pPopUpPanel = nullptr;
    TextureComponent* m_pReadyGameStart = nullptr;
    TextureComponent* m_pCutScene = nullptr;

    TextureComponent* m_pBackground1 = nullptr;
    TextureComponent* m_pBackground2 = nullptr;


    float m_elapsedTime = 0.f;

    bool m_bPopUpState = false;

    bool m_playTime1 = false;
    bool m_playTime2 = false;
};