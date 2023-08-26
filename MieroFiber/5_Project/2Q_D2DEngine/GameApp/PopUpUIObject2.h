#pragma once
#include "../Engine/GameObject.h"

enum eEventType;
class ButtonUI;
class PopUpUI;

class PopUpUIObject2
    : public GameObject
{
public:
    bool Initialize() final;
    void Update(const float deltaTime) final;

public:
    ButtonUI* GetOkayButton() { return m_pOkayButton; }

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

