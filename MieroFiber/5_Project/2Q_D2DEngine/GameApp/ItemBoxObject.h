#pragma once
#include "../Engine/GameObject.h"

class TextureComponent;
class BoxCollider2D;

enum class eItemBoxType
{
	THROW,
	INSTALLATION,
	REINFORCED,

	END_ITEMBOX
};

class ItemBoxObject
	: public GameObject
{
private:
	eItemBoxType m_ItemBoxType;
	TextureComponent* m_pTextureComponent = nullptr;
	BoxCollider2D* m_pBoxCollider = nullptr;

public:
	void SetItemBoxType(eItemBoxType itemBoxType) { m_ItemBoxType = itemBoxType; }
	eItemBoxType GetItemBoxType() { return m_ItemBoxType; }

public:
	bool Initialize() final;
	void Update(const float deltaTime) final;
};