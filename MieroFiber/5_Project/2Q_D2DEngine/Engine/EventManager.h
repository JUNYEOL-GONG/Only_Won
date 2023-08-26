#pragma once

#include "Event.h"

#include <list>
#include <map>

/// <summary>
///	�̺�Ʈ Ÿ�Կ� enum
///	�̺�Ʈ �Ǻ��� ����Ѵ�, Ÿ�� ���� ���ڷ� ������ �������� �ٸ���
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

	// PlayerObject - ����ȭ
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
	// ���� ��ȯ
	ChangeWorld_Start,
	ChangeWorld,
	ChangeWorldToMain,				// MainWorld�� ��ȯ(����ȭ��)
	ChangeWorldToInstruction,		// GameMethonWorld�� ��ȯ(���ӹ��)
	ChangeWorldToMadeBy,			// MadeByWorld�� ��ȯ(����)
	ChangeWorldToGameSetting,		// GameSettingWorld�� ��ȯ(���Ӽ���)
	ChangeWorldToInGame,			// InGameWorld�� ��ȯ(�ΰ���)
	ChangeWorldToInGame2,			// InGameWorld�� ��ȯ(�ΰ���2)
	GameEnd,			// InGameWorld�� ��ȯ(�ΰ���)
	ChangeWorld_End,

	// ������ ������ �԰� ������ ��
	DeleteThrow,
	DeleteInstallation,
	DeleteReinforced,
	DeleteMoney,

	// ������ �Ծ��� �� �����Կ� ���̱�
	ItemVisible,
	ItemInVisible,

	// World Pause ���� ���� ON OFF
	PauseOn,
	PauseOff,

	// �Ͻ����� �˾�
	CheckQuitGame,
	Continue,
	CheckQuitGame2,
	Continue2,

	// �ʼ���
	SetMap1,
	SetMap2,

	// ���ӹ�� ���� �ؽ��� ��ȯ
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
///	1. RegisterListener()�� �̺�ƮŸ�԰� �����ʸ� ���
///	2. SendEvent()�� �̺�Ʈ�� �����ϰ� EventManager.Update()���� ������ m_EventList�� �߰�
///	3. Update()���� �̺�Ʈ Execute
/// </summary>
class EventManager
{
private:
	// �����ڿ� �� �̺�Ʈ�� ���� ���
	std::multimap<eEventType, EventListener*> m_WholeEvents;
	// ó���Ǿ�� �ϴ� �̺�Ʈ ť
	std::list<Event> m_EventList;

	/// ChagneWorld Event�� ��� WorldManager::ChangeWorld()�� �����ϸ鼭 �̺�Ʈ�Ŵ����� �ʱ�ȭ�Ѵ�.
	///	�� ��� �߻��ϴ� ������ ���� ����ó���� �������
	bool m_bChangingWorld = false;
	bool m_bInitializing = false;

private:
	static EventManager* m_pInstance;

public:
	static EventManager* GetInstance();

	// �ش� eventType�� listener ���, ����
	void RegisterListener(eEventType eventType, EventListener* listener);
	void UnregisterListener(eEventType eventType, EventListener* listener);

	// �ش� listener�� ��ϵ� ��� �̺�Ʈ ����
	void UnregisterAll(EventListener* listener);
	// �̺�Ʈ ���, �̺�Ʈ ����Ʈ �ʱ�ȭ
	void ClearEvents();

	void Initialize();	// ���� Initialize���� ��ϵ� Event ����
	void Update();	// �̺�Ʈ ť ����
	void Finalize();

	// �̺�Ʈ Ÿ�� �� �̺�Ʈ�� �ʿ��� ���� ����
	void SendEvent(eEventType eventId);
	void SendEvent(eEventType eventId, framework::EVENT_ANIMATION_INFO animationInfo);
	void SendEvent(eEventType eventId, framework::EVENT_MOVEMENT_INFO movementInfo);
	void SendEvent(eEventType eventId, GROUP_TYPE group, GameObject* obj);
	void SendEvent(eEventType eventId, UINT playerNum, eItemBoxType itemBoxType, eItemType itemType);
	void SendEvent(eEventType eventId, int hp);
	void SendEvent(eEventType eventId, UINT playerNum, framework::Vector2D playerLocation, framework::Vector2D lookDirection);

private:
	// �̹� ��ϵ� �̺�Ʈ���� Ȯ��
	bool IsRegistered(eEventType eventType, EventListener* listener);

	// �̺�Ʈ ���࿡ ���
	void ExecuteEvent(Event* event);
};

