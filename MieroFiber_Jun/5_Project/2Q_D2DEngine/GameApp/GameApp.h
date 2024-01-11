#pragma once

#include "../Engine/CommonApp.h"

class World;

/// <summary>
///	실제 구동되는 게임 애플리케이션
/// 매니저들을 Init, Update, Render한다.
/// </summary>
class GameApp
	: public CommonApp
{
public:
	GameApp(HINSTANCE hInstance);
	~GameApp() final;

	void Update() final;
	void Render() final;
	bool Initialize() final;
	void Finalize() final;
};