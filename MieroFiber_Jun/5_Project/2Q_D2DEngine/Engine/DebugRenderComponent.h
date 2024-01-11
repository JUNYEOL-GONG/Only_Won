#pragma once

#include "RenderComponent.h"

/// <summary>
/// 디버그용 렌더링 컴포넌트
/// </summary>
class DebugRenderComponent :
    public RenderComponent
{
private:
    D2D1::ColorF m_Color = D2D1::ColorF::Black;

public:
    void SetColor(D2D1::ColorF color) { m_Color = color; }

public:
    void Render(ID2D1RenderTarget* pRenderTarget) final;

};