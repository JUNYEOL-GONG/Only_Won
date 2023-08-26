#pragma once

#include "../Engine/GameObject.h"

#include "../Engine/InputManager.h"

class PlayerObject;
class TextureComponent;

enum class eItemBoxType;

enum class eItemType
{
	NOTHING,

	// THROW
	THROW_START,
	PunchItem,
	SnowItem,
	StungunItem,
	BottleItem,
	THROW_END,

	// INSTALLATION
	INSTALLATION_START,
	SnareItem,
	TrapItem,
	INSTALLATION_END,

	// REINFORCED
	REINFORCED_START,
	ShoeItem,
	WaveItem,
	TransparencyItem,
	REINFORCED_END,

	END
};

/// <summary>
/// ItemObject의 원형
/// 인스턴스화하지 않는다
/// </summary>
class ItemObject
	: public GameObject
{
protected:
	GameObject* m_pOwnerPlayer = nullptr;
	TextureComponent* m_pTextureComponent = nullptr;
	eItemBoxType m_ItemBoxType;							// 아이템 타입(유형)
	eItemType m_ItemType;								// 아이템 종류(이름)

	float m_Range = 0.f;								// 투척형 아이템 유효 범위
	int m_Damage = 0;									// 데미지
	int m_DropedMoney = 0;								// 피격시 드랍하는 돈 개수
	float m_Duration = 0.f;								// 설치형, 강화형 아이템 지속 시간
	eKeyState m_UsedTiming = eKeyState::NONE;			// 아이템 사용시점의 입력상태

	// 아이템 테이블에 있는 투사체 속도는 movementComponent 통해 조절

public:
	GameObject* GetOwnerPlayer() { return m_pOwnerPlayer; }
	void SetOwnerPlayer(GameObject* pOwnerPlayer) { m_pOwnerPlayer = pOwnerPlayer; }

	eItemType GetItemType() { return m_ItemType; }
	void SetItemType(eItemType itemType) { m_ItemType = itemType; }

	eItemBoxType GetItemBoxType() { return m_ItemBoxType; }
	void SetItemBoxType(eItemBoxType itemBoxType) { m_ItemBoxType = itemBoxType; }

	int GetDamage() { return m_Damage; }
	eKeyState GetUsedTiming() { return m_UsedTiming; }
};