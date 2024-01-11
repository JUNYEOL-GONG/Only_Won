#include "pch.h"
#include "TextureAsset.h"

#include "CommonApp.h"

TextureAsset::~TextureAsset()
{
	// ���丮�� ���ؼ� �����Ѵ�.
	CommonApp::m_pInstance->GetRenderer()->ReleaseD2DBitmapFromFile(m_pBitmap);
}

void TextureAsset::SetBitmapFilePath(const WCHAR* szFilePath)
{
	m_BitmapFilePath = szFilePath;
}

void TextureAsset::Build()
{
	CommonApp::m_pInstance->GetRenderer()->CreateD2DBitmapFromFile(m_BitmapFilePath.c_str(), &m_pBitmap);
}
