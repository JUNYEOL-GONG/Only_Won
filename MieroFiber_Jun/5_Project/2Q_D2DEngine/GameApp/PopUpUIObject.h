#pragma once
#include "../Engine/EventListener.h"
#include "../Engine/GameObject.h"

enum eEventType;
class ButtonUI;
class PopUpUI;

class PopUpUIObject
	: public GameObject
{
public:
    bool Initialize() final;
    void Update(const float deltaTime) final;

public:
    void SetPopUpUIEventType(eEventType eventType) { m_popupEventType = eventType; };
    eEventType GetPopUpUIEventType() { return m_popupEventType; }

private:
    PopUpUI* m_pPopUpPanel = nullptr;
    ButtonUI* m_pOkayButton = nullptr;
    ButtonUI* m_pCancelPanel = nullptr;

    // 팝업이 되어있는지.
    bool m_bPopUpState = false;

    eEventType m_popupEventType;
};

