#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/EventListener.h"

class TextureComponent;
class ButtonUI;
class PanelUI;

class GameSettingUIObject
	: public GameObject
	, public EventListener
{
private:
    TextureComponent* m_pStartTexture = nullptr;
    TextureComponent* m_pActiveStartTexture = nullptr;

    PanelUI* m_pRootPanel = nullptr;
    PanelUI* m_pGameSettingPanel = nullptr;
    TextureComponent* m_pBackgroundTexture = nullptr;

    ButtonUI* m_pSelectMap1 = nullptr;
    TextureComponent* m_pSmallMap1Texture = nullptr;

    ButtonUI* m_pSelectMap2 = nullptr;
    TextureComponent* m_pSmallMap2Texture = nullptr;

    ButtonUI* m_pSelectMap3 = nullptr;
    TextureComponent* m_pSmallMap3Texture = nullptr;

    PanelUI* m_pMapImage = nullptr;
    TextureComponent* m_pBigMapTexture = nullptr;

    TextureComponent* m_pUsingItemTexture = nullptr;
    TextureComponent* m_pLogoTexture = nullptr;

    ButtonUI* m_pStartButton = nullptr;

    ButtonUI* m_pOkayButton = nullptr;
    ButtonUI* m_pCancelButton = nullptr;

public:
    bool Initialize() final;
    void Update(const float deltaTime) final;

    void HandleEvent(Event* event) final;
};