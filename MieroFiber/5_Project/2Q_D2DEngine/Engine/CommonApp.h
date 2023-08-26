#pragma once
#define MAX_LOADSTRING (100)

#include "TimeManager.h"
#include "WorldManager.h"

#include "../D2DRenderer/D2DRenderer.h"

class SoundManager;

/// <summary>
/// 게임에서 사용할 공통적인 처리를 하는 class
/// 실제로 만들 게임은 CommonApp을 상속 받아 부가적인 처리를 한다.
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
	static CommonApp* m_pInstance;	// 매 프레임 돌아야하기 때문에 public으로 선언

protected:
	HACCEL m_hAccelTable;
	MSG m_msg;
	HINSTANCE m_hInstance;
	WCHAR m_szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
	WCHAR m_szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
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