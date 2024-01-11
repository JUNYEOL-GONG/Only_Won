#pragma once
#include "../Engine/EventListener.h"
#include "../Engine/EventManager.h"
#include "../Engine/GameObject.h"

class TextureComponent;
enum eEventType;

class PanelUI;
class ButtonUI;

class InGamePopUpUI1
	: public GameObject
	, public EventListener
{
public:
    bool Initialize() final;
    void Update(const float deltaTime) final;

    void HandleEvent(Event* event) final;

public:
    void SetPopUpUIEventType(eEventType eventType) { m_popupEventType = eventType; };
    eEventType GetPopUpUIEventType() { return m_popupEventType; }

private:
    PanelUI* m_pRootPanel = nullptr;
    PanelUI* m_pPopUpPanel = nullptr;

    PanelUI* m_pPausePanel = nullptr;
    TextureComponent* m_pBackgroundTexture = nullptr;

    TextureComponent* m_pPauseTexture = nullptr;

    // 첫번째
    // 이어하기
    TextureComponent* m_pContinueTexture = nullptr;
    TextureComponent* m_pActiveContinueTexture = nullptr;
    ButtonUI* m_pContinueButton = nullptr;

    // 메인메뉴
    TextureComponent* m_pMainMenuTexture = nullptr;
    TextureComponent* m_pActiveMainMenuTexture = nullptr;
    ButtonUI* m_pMainMenuButton = nullptr;
    
    // 두번째
    // 텍스쳐
    TextureComponent* m_pGoToMainMenu = nullptr;

    // 예
    ButtonUI* m_pYesButton = nullptr;
    TextureComponent* m_pYesTexture = nullptr;
    TextureComponent* m_pActiveYesTexture = nullptr;

    // 아니요
    ButtonUI* m_pNoButton = nullptr;
    TextureComponent* m_pNoTexture = nullptr;
    TextureComponent* m_pActiveNoTexture = nullptr;


    // 팝업이 되어있는지.
    bool m_bPopUpState = false;

    eEventType m_popupEventType = eEventType::P1SetDefaultAnimation;
};