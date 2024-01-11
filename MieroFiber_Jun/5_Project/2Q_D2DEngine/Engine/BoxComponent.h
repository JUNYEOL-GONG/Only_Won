#pragma once
#include "RenderComponent.h"

/// <summary>
///	¹Ú½º ÇÏ³ª ¶î·Õ ±×¸®±â
/// </summary>
class BoxComponent
	: public RenderComponent
{
private:
	framework::Vector2D m_Extend = { 0.f, 0.f };
	D2D1::ColorF m_Color = D2D1::ColorF::Black;

public:
	framework::Vector2D GetExtend() const { return m_Extend; }
	void SetExtend(float x, float y) { m_Extend = { x,y }; }
	void SetColor(D2D1::ColorF color) { m_Color = color; }

public:
	void Render(ID2D1RenderTarget* pRenderTarget) final;
};