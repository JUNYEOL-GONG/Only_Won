#pragma once

#include <list>

/// <summary>
/// Renderer�� �̱������� ������ �ʱ����� �̱����� CommonApp(Core)�� Ȱ���Ѵ�.
/// GameApp �ܿ����� �������� ���縦 ����� �ϰ�,
/// Engine������ �������� ���縦 �ȴ�.
/// </summary>
class D2DRenderer
{
public:
	~D2DRenderer();

private:
	ID2D1HwndRenderTarget* m_pD2DRenderTarget = nullptr;

	// �⺻ D2D ����� ���� Factory�� �������̽� ������
	ID2D1Factory* m_pD2DFactory = nullptr;
	// ����Ÿ�ٿ��� ������ �귯���� �������̽� ������
	ID2D1SolidColorBrush* m_pBrush = nullptr;
	// �ؽ�Ʈ ����� ���� ��ü �������̽� ������
	IDWriteFactory* m_pDWriteFactory = nullptr;
	IDWriteTextFormat* m_pDWriteTextFormat = nullptr;
	// WIC �������̽�
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