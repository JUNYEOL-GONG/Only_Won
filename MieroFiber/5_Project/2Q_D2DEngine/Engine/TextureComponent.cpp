#include "pch.h"
#include "TextureComponent.h"

#include "CommonApp.h"
#include "TextureAsset.h"

TextureComponent::TextureComponent()
{
	m_pTextureAsset = new TextureAsset;
}

TextureComponent::~TextureComponent()
{
	delete m_pTextureAsset;
}

void TextureComponent::Render(ID2D1RenderTarget* pRenderTarget)
{
	if(!m_bIsShow)
		return;

	if(m_bMirrorX)
	{
		m_WorldTransform = D2D1::Matrix3x2F::Scale(D2D1::SizeF(-1.f, 1.f))
			* m_WorldTransform;
	}

	if (m_bMirrorY)
	{
		m_WorldTransform = D2D1::Matrix3x2F::Scale(D2D1::SizeF(1.f, -1.f))
			* m_WorldTransform;
	}

	m_FinalTransform = m_WorldTransform * GetCameraTransform();
	pRenderTarget->SetTransform(m_FinalTransform);

	// 비트맵 사이즈
	const D2D1_SIZE_F bitmapSize = m_pTextureAsset->m_pBitmap->GetSize();

	if(m_scaleX == 1.f && m_scaleY == 1.f)
	{
		pRenderTarget->DrawBitmap(
			m_pTextureAsset->m_pBitmap,
			{ -bitmapSize.width / 2.f, -bitmapSize.height / 2.f, bitmapSize.width / 2.f, bitmapSize.height / 2.f },
			1.f,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			{ 0.f, 0.f, bitmapSize.width, bitmapSize.height });
	}
	else
	{
		pRenderTarget->DrawBitmap(
			m_pTextureAsset->m_pBitmap,
			 { -bitmapSize.width / 2.f, -bitmapSize.height / 2.f, (bitmapSize.width * m_scaleX) / 2.f, bitmapSize.height / 2.f},
			1.f,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			{ 0.f, 0.f, bitmapSize.width, bitmapSize.height });
	}

	/*
	SetColor(D2D1::ColorF::Red);
	CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);

	// Draw Text Object Name
	pRenderTarget->DrawTextW(
		m_pOwner->GetName().c_str(),
		static_cast<UINT32>(m_pOwner->GetName().length()),
		CommonApp::m_pInstance->GetTextFormat(),
		D2D1::RectF(-50, -150, 50, -50),
		CommonApp::m_pInstance->GetBrush()
	);
	*/

	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void TextureComponent::SetTextureAsset(const WCHAR* szFilePath, const std::wstring textureName)
{
	m_pTextureAsset->SetBitmapFilePath(szFilePath);
	m_pTextureAsset->Build();
}