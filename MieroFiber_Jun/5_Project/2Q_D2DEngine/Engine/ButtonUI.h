#pragma once
#include "UI.h"

#include "EventManager.h"
class ButtonUI
    : public UI
{
public:
    //void CheckMouseOn() override {};
    //void MouseLbtnDown() override {};
    //void MouseLbtnUp() override {};
    //void MouseLbtnClicked() override;

    void SetButtonEventType(eEventType _func);
    eEventType GetButtonEventType() { return m_buttonEventType; }

public:
    bool Initialize() override final;
    void Update(const float deltaTime) override final;
    void Render(ID2D1RenderTarget* pRenderTarget) override final;

private:
    eEventType m_buttonEventType;

};

