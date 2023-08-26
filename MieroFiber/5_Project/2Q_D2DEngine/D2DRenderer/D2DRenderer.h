#pragma once

#include <list>

/// <summary>
/// Renderer를 싱글톤으로 만들지 않기위해 싱글톤인 CommonApp(Core)를 활용한다.
/// GameApp 단에서는 렌더러의 존재를 몰라야 하고,
/// Engine까지만 렌더러의 존재를 안다.
/// </summary>
class D2DRenderer
{
public:
	~D2DRenderer();

private:
	ID2D1HwndRenderTarget* m_pD2DRenderTarget = nullptr;

	// 기본 D2D 사용을 위한 Factory의 인터페이스 포인터
	ID2D1Factory* m_pD2DFactory = nullptr;
	// 렌더타겟에서 생성한 브러시의 인터페이스 포인터
	ID2D1SolidColorBrush* m_pBrush = nullptr;
	// 텍스트 출력을 위한 객체 인터페이스 포인터
	IDWriteFactory* m_pDWriteFactory = nullptr;
	IDWriteTextFormat* m_pDWriteTextFormat = nullptr;
	// WIC 인터페이스
	IWICImagingFactory* m_pIWICImagingFactory = nullptr;

	std::list<std::pair<std::wstring, ID2D1Bitmap*>> m_SharingD2DBitmaps;

public:
	ID2D1HwndRenderTarget* GetRenderTarget() const { return m_pD2DRenderTarget; }
	ID2D1SolidColorBrush* GetBrush() const { return m_pBrush; }
	IDWriteTextFormat* GetTextFormat() const { return m_pDWriteTextFormat; }

	HRESULT Initialize();
	HRESULT CreateD2DBitmapFromFile(const std::wstring& strFilePath, ID2D1Bitmap** pID2DBitmap);
	void ReleaseD2DBitmapFromFile(ID2D1Bitmap* pID2D1Bitmap);
};