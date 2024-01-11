#pragma once

#include "RenderComponent.h"

class TextureAsset;

class TextureComponent
	: public RenderComponent
{
private:
	TextureAsset* m_pTextureAsset = nullptr;
	bool m_bMirrorX = false;
	bool m_bMirrorY = false;
	bool m_bIsShow = true;
	D2D1::ColorF m_Color = D2D1::ColorF::Black;

	float m_scaleX = 1.0f;
	float m_scaleY = 1.0f;

public:
	TextureComponent();
	~TextureComponent() override;

public:
	void SetTextureAsset(const WCHAR* szFilePath, const std::wstring textureName);
	void SetFlipX(bool flipX) { m_bMirrorX = flipX; }
	void SetFlipY(bool flipY) { m_bMirrorY = flipY; }
	void SetShowTexture(bool isShow) { m_bIsShow = isShow; }
	void SetColor(D2D1::ColorF color) { m_Color = color; }
	void SetScale(float scaleX, float scaleY) { m_scaleX = scaleX; m_scaleY = scaleY; };

public:
	void Render(ID2D1RenderTarget* pRenderTarget) final;
};

