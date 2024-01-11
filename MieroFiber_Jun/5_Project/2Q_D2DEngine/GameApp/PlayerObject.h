#pragma once

#include "../Engine/GameObject.h"
#include "../Engine/EventListener.h"

#include "ItemBoxObject.h"
#include "ItemObject.h"

class ItemObject;
class AnimationComponent;
class PlayerMovement;
class FSMComponent;
class FSM;
class DirectionComponent;
class BoxCollider2D;

enum class eItemBoxType;

/// <summary>
/// �÷��̾� ������Ʈ
///	1P ~ 4P ��� ������ Ŭ������ ���
/// </summary>
class PlayerObject final
	: public GameObject
	, public EventListener
{
private:
	// ItemType�� ���ؼ� ���� ����ϴ� ������ �ν��Ͻ� �������ֱ�
	eItemType m_OwnedItem[static_cast<UINT>(eItemBoxType::END_ITEMBOX)] = {};	// ��ô��, ��ȭ��, ��ġ��
	UINT m_PlayerNumber = 0;
	int m_HP = 5;
	int m_Money = 0;
	int m_Score = 0;

	// ���� �Ҹ� �ѹ��� ��µǰ� �Ϸ���
	bool m_stunPlayTime = false;

	bool m_bStun = false;
	float m_StunTimer = 0.f;

	bool m_bFalling = false;
	float m_FallingTimer = 0.f;
	float m_MyScale = 1.f;
	float m_MyAngle = 0.f;

	bool m_bHitted = false;	// ����

	bool m_bSlow = false;		// Slow ����Ʈ ���
	bool m_bBlooding = false;	// ���� ����Ʈ ���
	bool m_bNegative = false;	// Negative ����Ʈ ���

	bool m_bTransparencing = false;	// ����ȭ ������

	int m_ItemUsageCnt[static_cast<UINT>(eItemType::END)] = {};
	int m_PlayerUsageCnt[static_cast<UINT>(eItemBoxType::END_ITEMBOX)] = {};
	bool m_IsItemUsable = false;
	float m_itemTimer = 0.f;
	float m_itemCoolTime = 0.2f;

	// �ʱ����� �տ� ���� �ִ���
	bool m_bIsLookFront = false;
	float m_bundleX = 30.f;
	float m_bundleY = 0.f;

	// �÷��̾�� Ÿ�̸�(2�ʵ��� �� ������ ü��ȸ���ϰ�)
	float m_playerTimer = 0.f;

	framework::Vector2D m_truckPosition = { 1650.f, 540.f };

public:
	bool m_IsVibration = false;
	float m_ElapsedVibTime = 0.f;
	float m_VibrationTime = 0.f;
	WORD m_LeftVibPower = 0;
	WORD m_RightVibPower = 0;
	bool m_IsTrainVib = false;
	bool m_IsWinnableVib = false;

private:
	framework::Vector2D m_SpawnPosition = { 300.f, 300.f };		// �÷��̾� ���� ��ġ
	framework::Vector2D m_MoveDirToSpawn = { 0.f, 0.f };		// �÷��̾� ����� ���ư��� �ӵ�
	float m_DeadSpeed = 1000.f;										// �÷��̾� ����� ���ư��� �ӷ�
	float m_DeadRotation = 2000.f;									// �÷��̾� ����� ���ư��� ȸ�� �ӵ�

	AnimationComponent* m_pAnimComponent = nullptr;
	TextureComponent* m_pBundleTexture = nullptr;
	TextureComponent* m_pShadowTexture = nullptr;
	PlayerMovement* m_pMovementComponent = nullptr;
	FSMComponent* m_pFSMComponent = nullptr;
	FSM* m_pPlayerFSM = nullptr;
	DirectionComponent* m_pDirectionComponent = nullptr;
	BoxCollider2D* m_pBoxColliderFull = nullptr;
	BoxCollider2D* m_pBoxColliderPart = nullptr;	// ��, ���� ������ �� ������ �ݶ��̴�

	ItemObject* m_pCreatedItem = nullptr;

	AnimationComponent* m_pSlowAnimation = nullptr;
	AnimationComponent* m_pBloodAnimation = nullptr;
	AnimationComponent* m_pNegativeAnimation = nullptr;

	TextureComponent* m_pWinnableTexture = nullptr;

public:
	~PlayerObject() final;

public:
	bool Initialize() final;
	void Update(const float deltaTime) final;

public:
	void OnCollisionEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter) final;
	void OnCollisionStay(Collider2D* otherCollision) final;
	void OnCollisionExit(Collider2D* otherCollision) final;

	void OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter) final;
	void OnTriggerStay(Collider2D* otherCollision) final;
	void OnTriggerExit(Collider2D* otherCollision) final;

public:
	void HandleEvent(Event* event) final;

public:
	void AttachFSM(FSM* fsm);
	void SendHPToFSM();

	UINT GetPlayerNumber() { return m_PlayerNumber; }

	void GetDamaged(int damage, int dropMoney);	// ������ ����ŭ HP-, MONEY-

	bool IsStun() { return m_bStun; }
	bool IsFalling() { return m_bFalling; }

	int GetMoney() { return m_Money; }

	int GetScore() { return m_Score; }

	BoxCollider2D* GetBoxColliderFull() const { return m_pBoxColliderFull; }
	BoxCollider2D* GetBoxColliderPart() const { return m_pBoxColliderPart; }

	bool GetLookFront() { return m_bIsLookFront; }

	eItemType GetOwnedItem(eItemType itemType) { return m_OwnedItem[static_cast<UINT>(itemType)]; }

	void PlaySlowEffect() { m_bSlow = true; }
	void PlayBloodEffect() { m_bBlooding = true; }
	void PlayNegativeEffect() { m_bNegative = true; }

private:
	void InitAnimationAsset();
	void InitTextureAsset();

	bool GetRandomItem(eItemBoxType itemBoxType);

	void CreateInstance(eItemType itemType);
	void UseThrowItem();
	void UseInstallationItem();
	void UseReinforcedItem();

public:
	void SetReinforcedNothing();
	void SetTransparencing(bool transparencing) { m_bTransparencing = transparencing; }

private:
	void ItemCoolTimer(float deltaTime);

	// �÷��̾ 2�ʵ��� �� �޴°� �Ǵ�.
	bool PlayerGetDamagedTimer(float deltaTime);

public:
	void SetBundleSize();
	void SetShadowAsset();

private:
	void CalculateDeadVelocity();									// �÷��̾� ����� ���ư��� �ӵ� ���

	void TurnOffPlayerMovement();									// �÷��̾� ������ ����
	void TurnOnPlayerMovement();									// �÷��̾� ������ �ѱ�

	void ResetDeadMove();
	bool IsOutOfRange();											// �÷��̾ �� ������ ������ ���� üũ

public:
	/// Stun ���� ������ HP <= 0
	/// �����ۿ����� GetDamaged�� ȣ��, Stun���� ���� ��ü�� Player ���ο���
	// �÷��̾� ���� ���·� �����
	void TurnToStun();

	// �÷��̾� ������ �������� �̵�
	void MoveToSpawnImmediately();
	void MoveToSpawnWithRotation(float deltaTime);

	// �÷��̾� ���� ���¿��� Ǯ��
	void RevivePlayer();

	// ���� / ������ ������ �� ȣ���ϴ� �Լ�
	void FallOff();

	// �÷��̾� ���ۺ��� ���鼭 �۾����� ����
	void FallOutEffect(float deltaTime, bool isCliff);

	// ���� �Լ�
	void SetVibration(float vibrationTime, WORD leftVibPower, WORD rightVibPower);
	void SetVibration2(DWORD playerNumber, float vibrationTime, WORD leftVibPower, WORD rightVibPower);
};