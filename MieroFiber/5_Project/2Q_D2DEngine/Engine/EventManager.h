#pragma once

#include "Event.h"

#include <list>
#include <map>

/// <summary>
///	이벤트 타입용 enum
///	이벤트 판별에 사용한다, 타입 별로 인자로 들어오는 정보들이 다르다
///	</summary>
enum eEventType
{
	// FSMComponent - AnimationComponent
	P1SetDefaultAnimation,
	P2SetDefaultAnimation,
	P3SetDefaultAnimation,
	P4SetDefaultAnimation,

	P1ChangeAnimation,
	P2ChangeAnimation,
	P3ChangeAnimation,
	P4ChangeAnimation,

	P1KeepAnimation,
	P2KeepAnimation,
	P3KeepAnimation,
	P4KeepAnimation,

	// FSMComponent - PlayerMovement
	P1TransperMovement,
	P2TransperMovement,
	P3TransperMovement,
	P4TransperMovement,

	// PlayerObject - FSMTransition
	P1TransperHP,
	P2TransperHP,
	P3TransperHP,
	P4TransperHP,

	// PlayerObject - 투명화
	Player1TransparentStart,
	Player1TransparentEnd,
	Player2TransparentStart,
	Player2TransparentEnd,
	Player3TransparentStart,
	Player3TransparentEnd,
	Player4TransparentStart,
	Player4TransparentEnd,
	FootprintVisible,
	FootprintInVisible,


	// UIComponent - ButtonUIComponent
	// 월드 변환
	ChangeWorld_Start,
	ChangeWorld,
	ChangeWorldToMain,				// MainWorld로 변환(메인화면)
	ChangeWorldToInstruction,		// GameMethonWorld로 변환(게임방법)
	ChangeWorldToMadeBy,			// MadeByWorld로 변환(제작)
	ChangeWorldToGameSetting,		// GameSettingWorld로 변환(게임설정)
	ChangeWorldToInGame,			// InGameWorld로 변환(인게임)
	ChangeWorldToInGame2,			// InGameWorld로 변환(인게임2)
	GameEnd,			// InGameWorld로 변환(인게임)
	ChangeWorld_End,

	// 스폰된 아이템 먹고 지워질 때
	DeleteThrow,
	DeleteInstallation,
	DeleteReinforced,
	DeleteMoney,

	// 아이템 먹었을 때 보관함에 보이기
	ItemVisible,
	ItemInVisible,

	// World Pause 월드 퍼즈 ON OFF
	PauseOn,
	PauseOff,

	// 일시정지 팝업
	CheckQuitGame,
	Continue,
	CheckQuitGame2,
	Continue2,

	// 맵선택
	SetMap1,
	SetMap2,

	// 게임방법 월드 텍스쳐 전환
	InstructionTexture,
	ControllerTexture,

	// DeleteObject
	DeleteGameObject,
};

enum class eItemType;
enum class eItemBoxType;

class EventListener;
class GameObject;

/// <summary>
///	1. RegisterListener()로 이벤트타입과 리스너를 등록
///	2. SendEvent()로 이벤트를 생성하고 EventManager.Update()에서 실행할 m_EventList에 추가
///	3. Update()에서 이벤트 Execute
/// </summary>
class EventManager
{
private:
	// 구독자와 그 이벤트에 대한 목록
	std::multimap<eEventType, EventListener*> m_WholeEvents;
	// 처리되어야 하는 이벤트 큐
	std::list<Event> m_EventList;

	/// ChagneWorld Event의 경우 WorldManager::ChangeWorld()를 수행하면서 이벤트매니저를 초기화한다.
	///	이 경우 발생하는 문제를 위한 예외처리용 멤버변수
	bool m_bChangingWorld = false;
	bool m_bInitializing = false;

private:
	static EventManager* m_pInstance;

public:
	static EventManager* GetInstance();

	// 해당 eventType의 listener 등록, 해제
	void RegisterListener(eEventType eventType, EventListener* listener);
	void UnregisterListener(eEventType eventType, EventListener* listener);

	// 해당 listener에 등록된 모든 이벤트 해제
	void UnregisterAll(EventListener* listener);
	// 이벤트 목록, 이벤트 리스트 초기화
	void ClearEvents();

	void Initialize();	// 각종 Initialize에서 등록된 Event 실행
	void Update();	// 이벤트 큐 실행
	void Finalize();

	// 이벤트 타입 및 이벤트에 필요한 정보 전달
	void SendEvent(eEventType eventId);
	void SendEvent(eEventType eventId, framework::EVENT_ANIMATION_INFO animationInfo);
	void SendEvent(eEventType eventId, framework::EVENT_MOVEMENT_INFO movementInfo);
	void SendEvent(eEventType eventId, GROUP_TYPE group, GameObject* obj);
	void SendEvent(eEventType eventId, UINT playerNum, eItemBoxType itemBoxType, eItemType itemType);
	void SendEvent(eEventType eventId, int hp);
	void SendEvent(eEventType eventId, UINT playerNum, framework::Vector2D playerLocation, framework::Vector2D lookDirection);

private:
	// 이미 등록된 이벤트인지 확인
	bool IsRegistered(eEventType eventType, EventListener* listener);

	// 이벤트 실행에 사용
	void ExecuteEvent(Event* event);
};

