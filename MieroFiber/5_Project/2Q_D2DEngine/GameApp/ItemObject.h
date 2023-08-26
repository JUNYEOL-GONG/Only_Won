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
/// ItemObject�� ����
/// �ν��Ͻ�ȭ���� �ʴ´�
/// </summary>
class ItemObject
	: public GameObject
{
protected:
	GameObject* m_pOwnerPlayer = nullptr;
	TextureComponent* m_pTextureComponent = nullptr;
	eItemBoxType m_ItemBoxType;							// ������ Ÿ��(����)
	eItemType m_ItemType;								// ������ ����(�̸�)

	float m_Range = 0.f;								// ��ô�� ������ ��ȿ ����
	int m_Damage = 0;									// ������
	int m_DropedMoney = 0;								// �ǰݽ� ����ϴ� �� ����
	float m_Duration = 0.f;								// ��ġ��, ��ȭ�� ������ ���� �ð�
	eKeyState m_UsedTiming = eKeyState::NONE;			// ������ �������� �Է»���

	// ������ ���̺� �ִ� ����ü �ӵ��� movementComponent ���� ����

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