#pragma once
#include "EventListener.h"
#include "RenderComponent.h"
#include "TextureComponent.h"

class TextureComponent;

class  UI
	: public RenderComponent
{
public:
    bool Initialize() override;
    void Update(const float deltaTime) override;
    void Render(ID2D1RenderTarget* pRenderTarget) override;

public:
    //framework::Vector2D  GetFinalPos() { return m_finalUIPos; }

    // UI들 정보 setting
    void SetColor(D2D1::ColorF color) { m_Color = color; }

    void FocusOn(D2D1::ColorF changeColor)	{ m_changeColor = m_Color; m_Color = changeColor; }
    void FocusOut() { m_Color = m_changeColor; }

    void FocusOn(std::vector<Component*>& childComponent)
    {
        dynamic_cast<TextureComponent*>(childComponent[1])->SetShowTexture(true);
    }
    void FocusOut(std::vector<Component*>& childComponent)
    {
        dynamic_cast<TextureComponent*>(childComponent[1])->SetShowTexture(false);
    }

    void SetWidth(const float width) { m_Width = width; }
    float GetWidth() const { return m_Width; }

    void SetHeight(const float height) { m_Height = height; }
    float GetHeight()const { return m_Height; }

    bool IsLbtnDown() const { return m_bLbtnDown; }
    bool IsMouseOn() const { return m_bMouseOn; }

    void SetLbtnDown(bool isDown) { m_bLbtnDown = isDown; }
    void SetMouseOn(bool isOn) { m_bMouseOn = isOn; }

    void SetYIndex(size_t y) { m_yIndex = y; }
    size_t GetYIndex() const { return m_yIndex; }

    void SetXIndex(size_t x) { m_xIndex = x; }
    size_t GetXIndex() const { return m_xIndex; }

    void SetActiveUI(bool activeUI) { m_bActiveUI = activeUI; }
    bool IsActiveUI() const { return m_bActiveUI; }


public:
    // 마우스가 UI에 있을 때
    virtual void CheckMouseOn();
    virtual void MouseLbtnDown() {};
    virtual void MouseLbtnUp() {};
    virtual void MouseLbtnClicked() {};

private:
    D2D1::ColorF m_changeColor = D2D1::ColorF::Black;

protected:
    // 마우스로 했을 때 내가 마지막으로 선택한 UI의 Location(Position)
	framework::Vector2D m_finalUIPos = {0,0};

    float m_Width = 0.f;
    float m_Height = 0.f;
    D2D1::ColorF m_Color = D2D1::ColorF::Black;

    bool m_bLbtnDown = false;
    bool m_bMouseOn = false;
    // Focus 할 수 있는 UI인지 아닌지
    bool m_bActiveUI = true;

    size_t m_xIndex = 0;
    size_t m_yIndex = 0;
};
