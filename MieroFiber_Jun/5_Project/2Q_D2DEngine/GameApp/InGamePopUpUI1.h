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

    // ù��°
    // �̾��ϱ�
    TextureComponent* m_pContinueTexture = nullptr;
    TextureComponent* m_pActiveContinueTexture = nullptr;
    ButtonUI* m_pContinueButton = nullptr;

    // ���θ޴�
    TextureComponent* m_pMainMenuTexture = nullptr;
    TextureComponent* m_pActiveMainMenuTexture = nullptr;
    ButtonUI* m_pMainMenuButton = nullptr;
    
    // �ι�°
    // �ؽ���
    TextureComponent* m_pGoToMainMenu = nullptr;

    // ��
    ButtonUI* m_pYesButton = nullptr;
    TextureComponent* m_pYesTexture = nullptr;
    TextureComponent* m_pActiveYesTexture = nullptr;

    // �ƴϿ�
    ButtonUI* m_pNoButton = nullptr;
    TextureComponent* m_pNoTexture = nullptr;
    TextureComponent* m_pActiveNoTexture = nullptr;


    // �˾��� �Ǿ��ִ���.
    bool m_bPopUpState = false;

    eEventType m_popupEventType = eEventType::P1SetDefaultAnimation;
};