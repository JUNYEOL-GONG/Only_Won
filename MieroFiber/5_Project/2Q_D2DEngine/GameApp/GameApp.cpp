#include "framework.h"
#include "GameApp.h"

#include "Resource.h"
// #include "TestWorld.h"
// #include "JunWorld.h"
// #include "ChaeWorld.h"
#include "GameSettingWorld.h"
#include "InGameWorld.h"
#include "InstructionWorld.h"
#include "MadeByWorld.h"
#include "MainWorld.h"

#include "../Engine/EventManager.h"
#include "../engine/InputManager.h"
#include "../Engine/SoundManager.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	// Debug Memory Leak Check at start point
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(2214);

    // 전역 문자열을 초기화합니다.
    GameApp App(hInstance);
    App.Initialize();
    App.Loop();

    // Debug Memory Leak Check at end point
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();

    return (int)1;
}

GameApp::GameApp(HINSTANCE hInstance)
    :CommonApp(hInstance)
{
    // GameApp에서 리소스를 얻어와 클래스 설정하기
    LoadStringW(hInstance, IDS_APP_TITLE, m_szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GAMEAPP, m_szWindowClass, MAX_LOADSTRING);

    m_wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMEAPP));
    m_wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
    m_wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    m_wcex.lpszMenuName = MAKEINTRESOURCEW(IDI_GAMEAPP);

    m_hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDI_GAMEAPP));
}

GameApp::~GameApp()
{

}

void GameApp::Update()
{
    CommonApp::Update();

    // WorldManager Update
    m_pWorldManager->Update(m_TimeManager.GetDeltaTime());

    // 월드매니저까지 CommonApp Update로 편입되면 EventManager도 같이 넣을 수 있을듯
    EventManager::GetInstance()->Update();
}

void GameApp::Render()
{
    m_pRenderTarget->BeginDraw();

    const D2D1::ColorF color(D2D1::ColorF::DarkGray);
    m_pRenderTarget->Clear(color);

    // WorldManager Render
    m_pWorldManager->Render();

    m_pRenderTarget->EndDraw();
}

bool GameApp::Initialize()
{
	const bool res = CommonApp::Initialize();
    assert(res);

    // 모든 월드 인스턴스 생성
    // World* pTestWorld = new TestWorld;
    // World* pJunWorld = new JunWorld;
    // World* pChaeWorld = new ChaeWorld;
    World* pMainWorld = new MainWorld;
    World* pInstructionWorld = new InstructionWorld;
    World* pMadeByWorld = new MadeByWorld;
    World* pGameSettingWorld = new GameSettingWorld;
    World* pInGameWorld = new InGameWorld;

    // 월드 등록
    // m_pWorldManager->RegisterWorld(pJunWorld, WORLD_TYPE::JUN);
    // m_pWorldManager->RegisterWorld(pChaeWorld, WORLD_TYPE::CHAE);
    // m_pWorldManager->RegisterWorld(pTestWorld, WORLD_TYPE::TEST);
    m_pWorldManager->RegisterWorld(pMainWorld, WORLD_TYPE::MAIN);
    m_pWorldManager->RegisterWorld(pInstructionWorld, WORLD_TYPE::INSTRUCTION);
    m_pWorldManager->RegisterWorld(pMadeByWorld, WORLD_TYPE::MADEBY);
    m_pWorldManager->RegisterWorld(pGameSettingWorld, WORLD_TYPE::GAMESETTING);
    m_pWorldManager->RegisterWorld(pInGameWorld, WORLD_TYPE::INGAME);

    // 기본 월드 설정 ** 여기서 자신의 월드 변경 **
    m_pWorldManager->SetDefaultWorld(pInGameWorld);

    // 월드 매니저 초기화
    m_pWorldManager->Initialize();

    // 이벤트 매니저 초기화
	EventManager::GetInstance()->Initialize();

    return false;
}

void GameApp::Finalize()
{
    EventManager::GetInstance()->Finalize();

    CommonApp::Finalize();
}