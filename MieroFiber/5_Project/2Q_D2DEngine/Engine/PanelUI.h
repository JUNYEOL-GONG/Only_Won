#pragma once
#include "UI.h"

class PanelUI
	: public UI
{
public:
    bool Initialize() final;
    void Update(const float deltaTime) final;
    void Render(ID2D1RenderTarget* pRenderTarget) final;
};