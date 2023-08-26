#pragma once
#define MAX_LOADSTRING (100)

#include "TimeManager.h"
#include "WorldManager.h"

#include "../D2DRenderer/D2DRenderer.h"

class SoundManager;

/// <summary>
/// ���ӿ��� ����� �������� ó���� �ϴ� class
/// ������ ���� ������ CommonApp�� ��� �޾� �ΰ����� ó���� �Ѵ�.
/// </summary>
class CommonApp
{
protected:
	D2DRenderer* m_pD2DRenderer = nullptr;
	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;
	ID2D1SolidColorBrush* m_pBrush = nullptr;
	IDWriteTextFormat* m_pTextFormat = nullptr;

protected:
	TimeManager m_TimeManager;
	WorldManager* m_pWorldManager;

public:
	D2DRenderer* GetRenderer() const { return m_pD2DRenderer; }
	ID2D1HwndRenderTarget* GetRenderTarget() const { return m_pRenderTarget; }
	ID2D1SolidColorBrush* GetBrush() const { return m_pBrush; }
	IDWriteTextFormat* GetTextFormat() const { return m_pTextFormat; }

protected:
	CommonApp(HINSTANCE hInstance);
	virtual ~CommonApp();

public:
	static HWND m_hWnd;
	static CommonApp* m_pInstance;	// �� ������ ���ƾ��ϱ� ������ public���� ����

protected:
	HACCEL m_hAccelTable;
	MSG m_msg;
	HINSTANCE m_hInstance;
	WCHAR m_szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
	WCHAR m_szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.
	WNDCLASSEXW m_wcex;
	int m_nCmdShow;

public:
	virtual bool Initialize();
	void Loop();
	virtual void Update();
	virtual void Render() abstract;
	virtual void Finalize();

public:
	BOOL GetClientRect(LPRECT lpRect);
	int MessageBoxComError(HRESULT hr);
	void CalculateFrameStats();

	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};