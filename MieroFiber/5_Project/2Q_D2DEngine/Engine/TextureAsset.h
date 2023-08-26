#pragma once
#include "ReferenceCounter.h"

class TextureAsset
	: public ReferenceCounter
{
public:
	virtual ~TextureAsset();

public:
	ID2D1Bitmap* m_pBitmap = nullptr;
	std::wstring m_BitmapFilePath;

	void SetBitmapFilePath(const WCHAR* szFilePath);
	void Build();
};

