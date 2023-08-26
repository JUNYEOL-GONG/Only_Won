#pragma once
#include "UI.h"

class TextureAsset;

class BarUI
	: public UI
{
private:
    
public:
    bool Initialize() final;
    void Update(const float deltaTime) final;
    void Render(ID2D1RenderTarget* pRenderTarget) final;
};
