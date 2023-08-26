#pragma once

#include "../Engine/CommonApp.h"

class World;

/// <summary>
///	���� �����Ǵ� ���� ���ø����̼�
/// �Ŵ������� Init, Update, Render�Ѵ�.
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