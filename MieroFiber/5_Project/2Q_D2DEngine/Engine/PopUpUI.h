#pragma once
#include "UI.h"

class PopUpUI :
    public UI
{
public:
    bool Initialize() final;
    void Update(const float deltaTime) final;
    void Render(ID2D1RenderTarget* pRenderTarget) final;

public:
    void SetPopUpState(bool state) { m_bPopUpState = state; }
    bool GetPopUpState() { return m_bPopUpState; }

private:
    // �˾��� �������� �ƴ���
    bool m_bPopUpState = false;
};

