#pragma once

#include "Bar2Object.h"
#include "ItemObject.h"
#include "ItemBoxObject.h"

#include "../Engine/EventListener.h"
#include "../Engine/World.h"

class Bar3Object;
class Bar4Object;
class InGameResultPopUp;
class InGamePopUpUI2;
class InGamePopUpUI1;
class Bar1Object;
class PlayerFSM1;
class PlayerFSM2;
class PlayerFSM3;
class PlayerFSM4;
class ShoeItem;
class PlayerObject;
class ItemSpawner;
class InGameUIObject;
class MoneyObject;
class ItemBoxObject;
class MoneySpawner;
class Background;
class ObstacleObject;
class TrafficObject;
class TrainSpawner;

class InGameWorld
	: public World
	, public EventListener
{
	/// 게임 종료 목표
private:
	int m_GoalScore = 20;		// 목표 금액
	int m_CurWinner = 0;		// 현재 1등 플레이어 -> playerNum(0~3) / Update에서 계속 조절
	int m_CurWinnerScore = 0;	// 현재 1등 플레이어의 누적 스코어		  / Update에서 계속 조절

public:
	int GetGoalScore() { return m_GoalScore; }
	int GetCurWinner() { return m_CurWinner; }
	int GetCurWinnerScore() { return m_CurWinnerScore; }

	// true면 게임 끝
	bool IsGameClear() { return m_CurWinnerScore >= m_GoalScore; }

private:
	framework::Vector2D m_Player1Point = { 320.f, 195.f };
	framework::Vector2D m_Player2Point = { 405.f, 195.f };
	framework::Vector2D m_Player3Point = { 270.f, 255.f };
	framework::Vector2D m_Player4Point = { 355.f, 255.f };

	InGameUIObject* m_pInGameUIObject = nullptr;

	// Background
	Background* m_pBackground = nullptr;

	// ObstacleObject
	ObstacleObject* m_pTunnel_1 = nullptr;
	ObstacleObject* m_pTunnel_2 = nullptr;
	ObstacleObject* m_pTunnel_3 = nullptr;
	ObstacleObject* m_pTunnel_4 = nullptr;
	ObstacleObject* m_pTunnel_5 = nullptr;
	ObstacleObject* m_pTunnel_6 = nullptr;

	ObstacleObject* m_pBarrel_1 = nullptr;
	ObstacleObject* m_pBarrel_2 = nullptr;
	ObstacleObject* m_pBarrel_3 = nullptr;
	ObstacleObject* m_pBarrel_4 = nullptr;
	ObstacleObject* m_pBarrel_5 = nullptr;
	ObstacleObject* m_pBarrel_6 = nullptr;
	ObstacleObject* m_pBox = nullptr;

	TrafficObject* m_pTraffic_1 = nullptr;
	TrafficObject* m_pTraffic_2 = nullptr;
	TrafficObject* m_pTraffic_3 = nullptr;

	ObstacleObject* m_pTruck = nullptr;

	// Player
	PlayerObject* m_pPlayerObject1 = nullptr;
	PlayerObject* m_pPlayerObject2 = nullptr;
	PlayerObject* m_pPlayerObject3 = nullptr;
	PlayerObject* m_pPlayerObject4 = nullptr;

	PlayerFSM1* m_pPlayerFSM1 = nullptr;
	PlayerFSM2* m_pPlayerFSM2 = nullptr;
	PlayerFSM3* m_pPlayerFSM3 = nullptr;
	PlayerFSM4* m_pPlayerFSM4 = nullptr;

	TrainSpawner* m_pTrainSpawner_1 = nullptr;
	TrainSpawner* m_pTrainSpawner_2 = nullptr;
	TrainSpawner* m_pTrainSpawner_3 = nullptr;

	MoneySpawner* m_pMoneySpawner = nullptr;
	ItemSpawner* m_pItemThrowSpawner = nullptr;
	ItemSpawner* m_pItemInstallationSpawner = nullptr;
	ItemSpawner* m_pItemReinforcedSpawner = nullptr;

	Bar1Object* m_pBar1Object = nullptr;
	Bar2Object* m_pBar2Object = nullptr;
	Bar3Object* m_pBar3Object = nullptr;
	Bar4Object* m_pBar4Object = nullptr;

	InGamePopUpUI1* m_pInGamePopUpUI1 = nullptr;
	InGamePopUpUI2* m_pInGamePopUpUI2 = nullptr;

	InGameResultPopUp* m_pInGameResultPopUp = nullptr;

	std::vector<PlayerObject*> m_PlayerList;
	std::vector<eItemType> m_ItemList[static_cast<UINT>(eItemBoxType::END_ITEMBOX)];

public:
	~InGameWorld() final;

	bool Initialize() final;
	void Update(const float deltaTime) final;
	void Finalize() final;

	void Enter() final;
	void Exit() final;

	eItemType GetRandomItem(eItemBoxType itemBoxType);
	std::vector<PlayerObject*>& GetPlayerList() { return m_PlayerList; }

	void HandleEvent(Event* event) final;
};