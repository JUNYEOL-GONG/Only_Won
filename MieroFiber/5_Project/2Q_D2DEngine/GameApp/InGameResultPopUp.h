#pragma once

#include "../Engine/GameObject.h"

class AnimationComponent;
class ButtonUI;
class PanelUI;
class TextureComponent;

class InGameResultPopUp
	: public GameObject
{
private:
	TextureComponent* m_pEndingBeforeTexture = nullptr;
	TextureComponent* m_pEndingAfterTexture = nullptr;

	AnimationComponent* m_pWinnerIdle = nullptr;
	TextureComponent* m_pExclamation = nullptr;
	TextureComponent* m_pWinnerTexture = nullptr;
	TextureComponent* m_pExitTexture = nullptr;

	// 루트 패널
	PanelUI* m_pRootPanel = nullptr;
	PanelUI* m_pMainResultPanel = nullptr;
	PanelUI* m_pResultPanel = nullptr;
	ButtonUI* m_pExitButton = nullptr;

	int m_winner;
	framework::Vector2D m_winnerPoint;

	float m_DelayTime = 0.f;

	bool m_musicPlay1 = false;
	bool m_musicPlay2 = false;

public:
	bool Initialize() final;
	void Update(const float deltaTime) final;

	void SetWinner(int winner, framework::Vector2D winnerPoint) { m_winner = winner; m_winnerPoint = winnerPoint; }
	void SetWinnerTextureAsset();
	void SetWinnerAnimtionAsset();
};

