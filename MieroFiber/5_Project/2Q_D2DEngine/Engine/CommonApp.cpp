#include "pch.h"
#include "CommonApp.h"

#include "InputManager.h"
#include "SoundManager.h"

#ifdef FRAMEWORK_DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif

CommonApp* CommonApp::m_pInstance = nullptr;
HWND CommonApp::m_hWnd;

LRESULT CALLBACK DefaultWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return CommonApp::m_pInstance->WndProc(hWnd, message, wParam, lParam);
}

CommonApp::CommonApp(HINSTANCE hInstance)
    : m_hInstance(hInstance)
    , m_szWindowClass(L"DefaultWindowClass")
    , m_szTitle(L"CommonApp")
{
    std::wstring str(__FUNCTIONW__);
    str += L"\n";
    OutputDebugString(str.c_str());

    CommonApp::m_pInstance = this;

    m_wcex.hInstance = hInstance;
    m_wcex.cbSize = sizeof(WNDCLASSEX);
    m_wcex.style = CS_HREDRAW | CS_VREDRAW;
    m_wcex.lpfnWndProc = DefaultWndProc;
    m_wcex.cbClsExtra = 0;
    m_wcex.cbWndExtra = 0;
    m_wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    m_wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    m_wcex.lpszClassName = m_szWindowClass;

    // ������ ����
    m_pD2DRenderer = new D2DRenderer;
}

CommonApp::~CommonApp()
{
    std::wstring str(__FUNCTIONW__);
    str += L"\n";
    OutputDebugString(str.c_str());
}

// ������ ������ ���Ӹ��� �ٸ��� ������ ������ ���, ����, ���̱⸸ �Ѵ�.
bool CommonApp::Initialize()
{
    /// ������ ����

	// ���
    RegisterClassExW(&m_wcex);

    const RECT rt = { 0, 0, ScreenWidth, ScreenHeight };
    // AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);

    // ȭ�� ũ�⸦ �������� �����츦 ����� ����
    const int screenCenterX = GetSystemMetrics(SM_CXSCREEN) / 2;
    const int screenCenterY = GetSystemMetrics(SM_CYSCREEN) / 2;
    const int windowWidth = rt.right - rt.left;
    const int windowHeight = rt.bottom - rt.top;
    const int startX = screenCenterX - windowWidth / 2;
    const int startY = screenCenterY - windowHeight / 2;

    // ������ ����
    m_hWnd = CreateWindowW(m_szWindowClass, m_szTitle, WS_POPUP,
        startX, startY,                       // ���� ��ġ
        rt.right - rt.left, rt.bottom - rt.top,                     // ����, ����
        nullptr, nullptr, m_hInstance, nullptr);

    if (!m_hWnd)
    {
        return false;
    }

    // ���Ӱ� �߰��� WinAPI �Լ��� (���� �ƴ�)
    SetForegroundWindow(m_hWnd);    // �����츦 �ֻ�����
    SetFocus(m_hWnd);               // �����쿡 ��Ŀ���� �ش�
    ShowCursor(TRUE);	            // Ŀ�� ���̱�

    // ������ ���̱�
    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

    /// �ܼ�â ����

    /*
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    COORD bufferSize;
    bufferSize.X = 80; // �ʰ� ���ϴ� �ʺ�
    bufferSize.Y = 40; // �ʰ� ���ϴ� ����
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    SMALL_RECT windowSize;
    windowSize.Left = 0;
    windowSize.Top = 0;
    windowSize.Right = bufferSize.X - 1;
    windowSize.Bottom = bufferSize.Y - 1;
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

    HWND consoleWindow = GetConsoleWindow();

    RECT consoleRect;
    GetWindowRect(consoleWindow, &consoleRect);

    int consoleWidth = consoleRect.right - consoleRect.left;
    int consoleHeight = consoleRect.bottom - consoleRect.top;

    int startX_2 = screenCenterX - consoleWidth / 2;
    int startY_2 = screenCenterY - consoleHeight / 2;
    SetWindowPos(consoleWindow, NULL, startX_2, startY_2, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    */

    // ������ ���
    HRESULT hr = m_pD2DRenderer->Initialize();
    if (FAILED(hr))
    {
        MessageBoxComError(hr);
    }
    m_pRenderTarget = m_pD2DRenderer->GetRenderTarget();
    m_pBrush = m_pD2DRenderer->GetBrush();
    m_pTextFormat = m_pD2DRenderer->GetTextFormat();

    // ���� �Ŵ��� ����
    m_pWorldManager = new WorldManager;

    m_TimeManager.Initialize();
    InputManager::GetInstance()->Initialize();
    SoundManager::GetInstance()->Initialize();

    return true;
}

void CommonApp::Loop()
{
    while (true)
    {
        if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
        {
            if (m_msg.message == WM_QUIT)
                break;

            if (!TranslateAccelerator(m_msg.hwnd, m_hAccelTable, &m_msg))
            {
                TranslateMessage(&m_msg);
                DispatchMessage(&m_msg);
            }
        }
        else
        {
            Update();
            Render();
        }
    }

    Finalize();
}

void CommonApp::Update()
{
    m_TimeManager.Update();
    InputManager::GetInstance()->Update();
    CalculateFrameStats();
}

void CommonApp::Finalize()
{
    // ��� ���� �ν��Ͻ� ����
    m_pWorldManager->Finalize();

    SoundManager::GetInstance()->Finalize();

    InputManager::GetInstance()->Finalize();

    // ����Ŵ������� Asset Release�� �ϱ� ������ D2DRenderer�� ���� ���Ŀ� Finalize �ؾ��Ѵ�
    delete m_pD2DRenderer;
}

BOOL CommonApp::GetClientRect(LPRECT lpRect)
{
    return ::GetWindowRect(m_hWnd, lpRect);
}

int CommonApp::MessageBoxComError(HRESULT hr)
{
    _com_error err(hr);
    return ::MessageBox(m_hWnd, err.ErrorMessage(), L"FAILED", MB_OK);
}

void CommonApp::CalculateFrameStats()
{
    float fps = m_TimeManager.GetFrameRate();
    std::wstring windowText;
    windowText.append(m_szTitle);
    windowText.append(L"   FPS: ");
    windowText.append(std::to_wstring(fps));
    SetWindowText(m_hWnd, windowText.c_str());
}

LRESULT CommonApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}