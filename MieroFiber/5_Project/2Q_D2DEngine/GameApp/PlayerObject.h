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
/// 플레이어 오브젝트
///	1P ~ 4P 모두 동일한 클래스를 사용
/// </summary>
class PlayerObject final
	: public GameObject
	, public EventListener
{
private:
	// ItemType을 통해서 실제 사용하는 시점에 인스턴스 생성해주기
	eItemType m_OwnedItem[static_cast<UINT>(eItemBoxType::END_ITEMBOX)] = {};	// 투척형, 강화형, 설치형
	UINT m_PlayerNumber = 0;
	int m_HP = 5;
	int m_Money = 0;
	int m_Score = 0;

	// 스턴 소리 한번만 출력되게 하려고
	bool m_stunPlayTime = false;

	bool m_bStun = false;
	float m_StunTimer = 0.f;

	bool m_bFalling = false;
	float m_FallingTimer = 0.f;
	float m_MyScale = 1.f;
	float m_MyAngle = 0.f;

	bool m_bHitted = false;	// 기차

	bool m_bSlow = false;		// Slow 이펙트 재생
	bool m_bBlooding = false;	// 출혈 이펙트 재생
	bool m_bNegative = false;	// Negative 이펙트 재생

	bool m_bTransparencing = false;	// 투명화 중인지

	int m_ItemUsageCnt[static_cast<UINT>(eItemType::END)] = {};
	int m_PlayerUsageCnt[static_cast<UINT>(eItemBoxType::END_ITEMBOX)] = {};
	bool m_IsItemUsable = false;
	float m_itemTimer = 0.f;
	float m_itemCoolTime = 0.2f;

	// 너구리가 앞에 보고 있는지
	bool m_bIsLookFront = false;
	float m_bundleX = 30.f;
	float m_bundleY = 0.f;

	// 플레이어당 타이머(2초동안 안 맞으면 체력회복하게)
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
	framework::Vector2D m_SpawnPosition = { 300.f, 300.f };		// 플레이어 스폰 위치
	framework::Vector2D m_MoveDirToSpawn = { 0.f, 0.f };		// 플레이어 사망시 날아가는 속도
	float m_DeadSpeed = 1000.f;										// 플레이어 사망시 날아가는 속력
	float m_DeadRotation = 2000.f;									// 플레이어 사망시 날아가는 회전 속도

	AnimationComponent* m_pAnimComponent = nullptr;
	TextureComponent* m_pBundleTexture = nullptr;
	TextureComponent* m_pShadowTexture = nullptr;
	PlayerMovement* m_pMovementComponent = nullptr;
	FSMComponent* m_pFSMComponent = nullptr;
	FSM* m_pPlayerFSM = nullptr;
	DirectionComponent* m_pDirectionComponent = nullptr;
	BoxCollider2D* m_pBoxColliderFull = nullptr;
	BoxCollider2D* m_pBoxColliderPart = nullptr;	// 덫, 오일 아이템 등 감지용 콜라이더

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

	void GetDamaged(int damage, int dropMoney);	// 지정된 수만큼 HP-, MONEY-

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

	// 플레이어가 2초동안 안 받는거 판단.
	bool PlayerGetDamagedTimer(float deltaTime);

public:
	void SetBundleSize();
	void SetShadowAsset();

private:
	void CalculateDeadVelocity();									// 플레이어 사망시 날아가는 속도 계산

	void TurnOffPlayerMovement();									// 플레이어 움직임 끄기
	void TurnOnPlayerMovement();									// 플레이어 움직임 켜기

	void ResetDeadMove();
	bool IsOutOfRange();											// 플레이어가 맵 밖으로 나갔을 때를 체크

public:
	/// Stun 상태 조건은 HP <= 0
	/// 아이템에서는 GetDamaged로 호출, Stun상태 진입 자체는 Player 내부에서
	// 플레이어 스턴 상태로 만들기
	void TurnToStun();

	// 플레이어 리스폰 지역으로 이동
	void MoveToSpawnImmediately();
	void MoveToSpawnWithRotation(float deltaTime);

	// 플레이어 스턴 상태에서 풀기
	void RevivePlayer();

	// 절벽 / 함정에 빠졌을 때 호출하는 함수
	void FallOff();

	// 플레이어 빙글빙글 돌면서 작아지는 연출
	void FallOutEffect(float deltaTime, bool isCliff);

	// 진동 함수
	void SetVibration(float vibrationTime, WORD leftVibPower, WORD rightVibPower);
	void SetVibration2(DWORD playerNumber, float vibrationTime, WORD leftVibPower, WORD rightVibPower);
};