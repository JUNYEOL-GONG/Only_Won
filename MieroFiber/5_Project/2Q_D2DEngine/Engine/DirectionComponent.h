#pragma once

#include "RenderComponent.h"

class TextureAsset;

/// <summary>
/// ��ü�� �̵� ���� �� �ٶ󺸴� ������ ǥ���ϴ� ������Ʈ
/// </summary>
class DirectionComponent :
    public RenderComponent
{
private:
	TextureAsset* m_pTextureAsset = nullptr;
	bool m_HasProjectile = true;
	float m_Width = 0.f;
	float m_Height = 0.f;
	D2D1::ColorF m_Color = D2D1::ColorF::Yellow;

public:
	DirectionComponent();
	~DirectionComponent() override;

	void SetWidth(const float width) { m_Width = width; }
	void SetHeight(const float height) { m_Height = height; }
	void SetColor(D2D1::ColorF color) { m_Color = color; }

public:
	void SetTextureAsset(const WCHAR* szFilePath, const std::wstring textureName);

public:
	void Render(ID2D1RenderTarget* pRenderTarget) final;
};