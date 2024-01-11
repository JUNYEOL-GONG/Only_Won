#include "framework.h"
#include "PlayerObject.h"

#include "InGameWorld.h"
#include "ItemBoxObject.h"
#include "MoneyObject.h"
#include "PunchItem.h"
#include "SnowItem.h"
#include "StungunItem.h"
#include "BottleItem.h"
#include "ShoeItem.h"
#include "TestWorld.h"
#include "TrapItem.h"
#include "WaveItem.h"
#include "ProjectileObject.h"
#include "SnareItem.h"
#include "TransparencyItem.h"

#include "InGameWorld.h"

#include "../Engine/InputManager.h"
#include "../Engine/EventManager.h"
#include "../Engine/AnimationComponent.h"
#include "../Engine/PlayerMovement.h"
#include "../Engine/FSMComponent.h"
#include "../Engine/DirectionComponent.h"
#include "../Engine/Collider2D.h"
#include "../Engine/BoxCollider2D.h"
#include "../Engine/TextureComponent.h"
#include "../Engine/World.h"
#include "../Engine/SoundManager.h"

PlayerObject::~PlayerObject()
{
}

bool PlayerObject::Initialize()
{
	m_Money = 0;
	m_Score = 0;

	// �÷��̾� ��ȣ ���� / ���� ��ġ ����
	if (this->m_name == L"Player1")
	{
		m_PlayerNumber = 0;
		m_SpawnPosition = { 320.f, 195.f };
	}
	else if (this->m_name == L"Player2")
	{
		m_PlayerNumber = 1;
		m_SpawnPosition = { 405.f, 195.f };
	}
	else if (this->m_name == L"Player3")
	{
		m_PlayerNumber = 2;
		m_SpawnPosition = { 270.f, 255.f };
	}
	else if (this->m_name == L"Player4")
	{
		m_PlayerNumber = 3;
		m_SpawnPosition = { 355.f, 255.f };
	}

	// ���� ������ �ʱ�ȭ
	for(auto& item : m_OwnedItem)
		item = eItemType::NOTHING;

	// AnimationComponent
	m_pAnimComponent = CreateComponent<AnimationComponent>(L"AnimationComponent");
	m_pAnimComponent->SetRelativeLocation(ScreenWidth / 2.f, ScreenHeight / 2.f);
	SetRootComponent(m_pAnimComponent);
	//m_pAnimComponent->LoadAnimationAsset(L"../Resource/ken.png", L"../Resource/TestWorld.WorldAsset");

	// EffectAnimation
	m_pSlowAnimation = CreateComponent<AnimationComponent>(L"SlowEffect");
	m_pSlowAnimation->SetLoop(false);
	m_pSlowAnimation->SetActive(false);
	m_pSlowAnimation->AttachToComponent(m_pAnimComponent);
	m_pBloodAnimation = CreateComponent<AnimationComponent>(L"BloodEffect");
	m_pBloodAnimation->SetLoop(false);
	m_pBloodAnimation->SetActive(false);
	m_pBloodAnimation->AttachToComponent(m_pAnimComponent);
	m_pNegativeAnimation = CreateComponent<AnimationComponent>(L"NegativeEffect");
	m_pNegativeAnimation->SetLoop(false);
	m_pNegativeAnimation->SetActive(false);
	m_pNegativeAnimation->SetRelativeLocation(0.f, -60.f);
	m_pNegativeAnimation->AttachToComponent(m_pAnimComponent);

	InitAnimationAsset();	// SetAnimationAsset

	// Texture
	m_pBundleTexture = CreateComponent<TextureComponent>(L"BundleTexture");
	m_pBundleTexture->SetTextureAsset(L"../Resource/bundle_S.png", L"");
	m_pBundleTexture->SetRelativeLocation(0.f, 0.f);
	m_pBundleTexture->AttachToComponent(m_pAnimComponent);

	m_pShadowTexture = CreateComponent<TextureComponent>(L"ShadowTexture");
	m_pShadowTexture->SetRelativeLocation(0.f, 60.f);
	m_pShadowTexture->AttachToComponent(m_pAnimComponent);
	InitTextureAsset();

	m_pWinnableTexture = CreateComponent<TextureComponent>(L"WinnableTexture");
	m_pWinnableTexture->SetTextureAsset(L"../Resource/Effect_Winnable.png", L"EffectWinnable");
	m_pWinnableTexture->SetVisible(false);
	m_pWinnableTexture->AttachToComponent(m_pAnimComponent);

	// ó���� �ʱ����� �տ� ���� �����ϱ� true
	m_bIsLookFront = true;

	// PlayerMovement
	m_pMovementComponent = CreateComponent<PlayerMovement>(L"PlayerMovement");

	// FSMComponent - AttatchFSM�� �Ʒ� �ν��Ͻ�ȭ�� Player���� �Ѵ�
	m_pFSMComponent = CreateComponent<FSMComponent>(L"FSMComponent");

	// DirectionComponent
	m_pDirectionComponent = CreateComponent<DirectionComponent>(L"DirectionComponent");
	m_pDirectionComponent->SetColor(D2D1::ColorF::Yellow);
	m_pDirectionComponent->AttachToComponent(m_pAnimComponent);

	// BoxColliderComponent_Full
	m_pBoxColliderFull = CreateComponent<BoxCollider2D>(L"Player_Body");
	m_pBoxColliderFull->SetRelativeLocation(0.f, 25.f);
	m_pBoxColliderFull->SetExtend(30.f, 25.f);
	m_pBoxColliderFull->AttachToComponent(m_pAnimComponent);

	// BoxColliderComponent_Part
	m_pBoxColliderPart = CreateComponent<BoxCollider2D>(L"Player_Part");
	m_pBoxColliderPart->SetCollisionType(COLLISION_TYPE::TRIGGER);
	m_pBoxColliderPart->SetRelativeLocation(0.f, 30.f);
	m_pBoxColliderPart->SetExtend(10.f, 15.f);
	m_pBoxColliderPart->AttachToComponent(m_pAnimComponent);

	// Event Listener Register
	EventManager::GetInstance()->RegisterListener(eEventType::Player1TransparentStart, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::Player1TransparentEnd, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::Player2TransparentStart, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::Player2TransparentEnd, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::Player3TransparentStart, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::Player3TransparentEnd, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::Player4TransparentStart, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::Player4TransparentEnd, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::P1SetDefaultAnimation, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::P1KeepAnimation, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::P1ChangeAnimation, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::P2SetDefaultAnimation, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::P2KeepAnimation, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::P2ChangeAnimation, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::P3SetDefaultAnimation, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::P3KeepAnimation, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::P3ChangeAnimation, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::P4SetDefaultAnimation, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::P4KeepAnimation, dynamic_cast<EventListener*>(this));
	EventManager::GetInstance()->RegisterListener(eEventType::P4ChangeAnimation, dynamic_cast<EventListener*>(this));

	// ������ ���Ƚ�� ����
	m_ItemUsageCnt[static_cast<UINT>(eItemType::PunchItem)] = 3;
	m_ItemUsageCnt[static_cast<UINT>(eItemType::SnowItem)] = 5;
	m_ItemUsageCnt[static_cast<UINT>(eItemType::StungunItem)] = 1;
	m_ItemUsageCnt[static_cast<UINT>(eItemType::BottleItem)] = 5;

	m_ItemUsageCnt[static_cast<UINT>(eItemType::SnareItem)] = 1;
	m_ItemUsageCnt[static_cast<UINT>(eItemType::TrapItem)] = 1;

	m_ItemUsageCnt[static_cast<UINT>(eItemType::ShoeItem)] = 1;
	m_ItemUsageCnt[static_cast<UINT>(eItemType::WaveItem)] = 1;
	m_ItemUsageCnt[static_cast<UINT>(eItemType::TransparencyItem)] = 1;

	const bool res = GameObject::Initialize();
	assert(res);

	return true;
}

void PlayerObject::Update(const float deltaTime)
{
	/// �÷��̾� �ð���ŭ ���� ����
	if (!m_IsTrainVib)
	{
		if (m_IsVibration)
		{
			m_ElapsedVibTime += deltaTime;

			InputManager::GetInstance()->SetControllerVib(m_PlayerNumber, m_LeftVibPower, m_RightVibPower);

			// ���� �ð� ������
			if (m_ElapsedVibTime >= m_VibrationTime)
			{
				m_ElapsedVibTime = 0.f;

				m_IsVibration = false;
				m_VibrationTime = 0.f;
				m_LeftVibPower = MinVibrationValue;
				m_RightVibPower = MinVibrationValue;
			}
		}
		else
		{
			InputManager::GetInstance()->SetControllerVib(m_PlayerNumber, MinVibrationValue, MinVibrationValue);
		}
	}

	/// ���� ���� ��ȯ Ȯ��
	if (m_HP <= 0 && !m_bStun)
	{
		TurnToStun();
	}

	/// LookDir�� ���� �������� �Դٸ� ���ٸ�
	const framework::Vector2D curLookDir = m_pMovementComponent->GetLookDir();
	if (curLookDir.Length() > 0.f)
	{
		if (curLookDir.y <= -0.8f)
		{
			m_bIsLookFront = false;
			m_pBundleTexture->SetRelativeLocation(0.f, m_bundleY);
		}
		else if (curLookDir.y >= 0.8f)
		{
			m_bIsLookFront = true;
			m_pBundleTexture->SetRelativeLocation(0.f, m_bundleY);
		}
		else
		{
			if (curLookDir.x >= 0.f)
			{
				m_bIsLookFront = true;
				m_pBundleTexture->SetFlipX(true);
				m_pBundleTexture->SetRelativeLocation(-m_bundleX, m_bundleY);
			}
			else
			{
				m_bIsLookFront = true;
				m_pBundleTexture->SetFlipX(false);
				m_pBundleTexture->SetRelativeLocation(m_bundleX, m_bundleY);
			}
		}
	}

	/// ������ �� ���ھ�� ��ȯ
	if (m_Money > 0 && framework::Vector2D::Distance(this->GetGameObjectLocation(), m_truckPosition) <= 120.f)
	{
		m_pMovementComponent->SetOriginalVelocityLimit(1.f + (static_cast<float>(m_Money) * 0.05f));

		if(m_PlayerNumber == 0)
			SoundManager::GetInstance()->PlayMusic(eSoundList::KeepEffect, eSoundChannel::Player1State, 0.7f);
		else if (m_PlayerNumber == 1)																			
			SoundManager::GetInstance()->PlayMusic(eSoundList::KeepEffect, eSoundChannel::Player2State, 0.7f);
		else if (m_PlayerNumber == 2)																			
			SoundManager::GetInstance()->PlayMusic(eSoundList::KeepEffect, eSoundChannel::Player3State, 0.7f);
		else if (m_PlayerNumber == 3)																			
			SoundManager::GetInstance()->PlayMusic(eSoundList::KeepEffect, eSoundChannel::Player4State, 0.7f);

		m_Score += m_Money;
		m_Money = 0;
	}

	/// �÷��̾� ȸ�� Ÿ�̸�
	if(!m_bStun && !PlayerGetDamagedTimer(deltaTime))
	{
		if (m_HP < 5)
		{
			m_HP++;
		}
	}

	/// ������ �� Ÿ�̸�
	ItemCoolTimer(deltaTime);

	/// ����Ʈ ���
	if(m_bSlow)
	{
		m_pSlowAnimation->SetActive(true);
		m_bSlow = false;
	}
	if(m_bBlooding)
	{
		m_pBloodAnimation->SetActive(true);
		m_bBlooding = false;
	}
	if(m_bNegative)
	{
		m_pNegativeAnimation->SetActive(true);
		m_bNegative = false;
	}

	/// ���� ������ �� + ��Ƶ� ���� ��ǥ�ݾ� �̻��� ��� ����Ʈ ���
	if ((m_Score + m_Money) >= dynamic_cast<InGameWorld*>(m_pOwnerWorld)->GetGoalScore())
	{
		m_pWinnableTexture->SetVisible(true);
		m_pWinnableTexture->AddRelativeRotation(deltaTime * 50.f);

		// ����...!
		if (!m_IsWinnableVib)
		{
			const std::vector<PlayerObject*> playerList = dynamic_cast<InGameWorld*>(this->GetOwnerWorld())->GetPlayerList();

			for (const auto& player : playerList)
			{
				// ���� ������ ��� �÷����̾�
				if (player != this)
					player->SetVibration(2.f, MaxVibrationValue, MaxVibrationValue);
				else
				{
					// player->SetVibration(2.f, MinVibrationValue, MaxVibrationValue / 2);
					this->SetVibration(2.f, MaxVibrationValue, MaxVibrationValue);
				}
			}

			m_IsWinnableVib = true;
		}
	}
	else
	{
		m_pWinnableTexture->SetVisible(false);

		m_IsWinnableVib = false;
	}

	/// ���� ���°� �ƴ� ���� ����
	if (!m_bStun)
	{
		// �������� ���� �ƴҶ�
		if (!m_bFalling)
		{
			// �� ������ ���� ���
			if (IsOutOfRange())
			{
				// ������
				FallOff();
			}
		}
		// �������� ����
		else
		{
			// �������� ������ ���
			if (IsOutOfRange())
			{
				FallOutEffect(deltaTime, true);
			}
			// �������� ������ ���
			else
			{
				// �������� ����
				FallOutEffect(deltaTime, false);
			}

			static float fallingTime = 0.f;
			fallingTime += deltaTime;

			// �÷��̾� �츮��
			if (fallingTime >= 2.f)
			{
				fallingTime = 0.f;
				RevivePlayer();
				MoveToSpawnImmediately();
			}
		}

		// ������ ��� ������ ���
		if (m_IsItemUsable)
		{
			// ������ ���
			if ((m_OwnedItem[static_cast<UINT>(eItemBoxType::THROW)] != eItemType::NOTHING) &&
				(InputManager::GetInstance()->IsRightTriggerPush(m_PlayerNumber)
					|| InputManager::GetInstance()->IsLeftTriggerPush(m_PlayerNumber)
					|| InputManager::GetInstance()->IsPadButtonPush(m_PlayerNumber, GamePadButtonCode::X)
					|| (this->GetName() == L"Player1" && InputManager::GetInstance()->IsKeyPush(eKeyCode::Z))))
			{
				UseThrowItem();
			}
			else if ((m_OwnedItem[static_cast<UINT>(eItemBoxType::INSTALLATION)] != eItemType::NOTHING) &&
				(InputManager::GetInstance()->IsPadButtonPush(m_PlayerNumber, GamePadButtonCode::A)
					|| (this->GetName() == L"Player1" && InputManager::GetInstance()->IsKeyPush(eKeyCode::X))))
			{
				UseInstallationItem();
			}
			else if ((m_OwnedItem[static_cast<UINT>(eItemBoxType::REINFORCED)] != eItemType::NOTHING) &&
				(InputManager::GetInstance()->IsPadButtonPush(m_PlayerNumber, GamePadButtonCode::B)
					|| (this->GetName() == L"Player1" && InputManager::GetInstance()->IsKeyPush(eKeyCode::C))))
			{
				UseReinforcedItem();
			}
		}
	}
	/// ���� ������ ��� ����
	else
	{
		// ������ �浹�� ���
		if (m_bHitted)
		{
			// Off All Collider
			const std::vector<Collider2D*> thiscolliders = this->GetComponents<Collider2D>();
			for (const auto thisCollider : thiscolliders)
			{
				thisCollider->SetCollisionType(COLLISION_TYPE::NONE);
			}
			MoveToSpawnWithRotation(deltaTime);
		}

		// ���� 3.5�� Ÿ�̸�
		m_StunTimer += deltaTime;

		if(m_StunTimer >= 3.5f)
		{
			m_StunTimer = 0.f;
			RevivePlayer();
			m_stunPlayTime = false;
		}
	}

	/// HP STUN ���¿� �ݿ��ϵ��� SendEvent
	SendHPToFSM();
	/// hp�� ���� �׸��� �ٲ��ֱ�
	SetShadowAsset();
	/// ������ ������ ���ϱ�
	SetBundleSize();

	/// ���������� ����ȭ�� �� �� ���̰� �ϱ�
	if (m_bTransparencing)
	{
		m_pBundleTexture->SetShowTexture(false);
		m_pShadowTexture->SetShowTexture(false);
	}
	else
	{
		// ���� �ϳ��� ���� ���� ������ �� ���̰� �ϱ�
		if(m_Money == 0)
			m_pBundleTexture->SetShowTexture(false);
		else
			m_pBundleTexture->SetShowTexture(true);
		m_pShadowTexture->SetShowTexture(true);
	}

	// �÷��̾� ������Ʈ ������Ʈ
	GameObject::Update(deltaTime);
}

void PlayerObject::OnCollisionEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{
	// ������ ��� �ݶ��̴����
	if (otherCollision->GetName() == L"BoxCollider2D_Head")
	{
		// Exit ó��
		thisCollision->TurnOff_IsCollision(otherCollision);

		iter->second = false;

		otherCollision->OnCollisionExit(thisCollision);
		thisCollision->OnCollisionExit(otherCollision);

		m_bHitted = true;

		// Money ���
		if(m_Money == 1)
		{
			GetDamaged(5, 1);
		}
		else
		{
			GetDamaged(5, m_Money / 2);
		}

		TurnToStun();
	}
	else if ((otherCollision->GetName() == L"Train_Body") && !m_IsTrainVib)
	{
		// �÷��̾� �ѹ��� ���� ���� ȿ�� �ֱ�
		InputManager::GetInstance()->SetControllerVib(m_PlayerNumber, MaxVibrationValue / 2, MaxVibrationValue / 2);
		m_IsTrainVib = true;
	}
}

void PlayerObject::OnCollisionStay(Collider2D* otherCollision)
{

}

void PlayerObject::OnCollisionExit(Collider2D* otherCollision)
{
	if (otherCollision->GetName() == L"Train_Body" && m_IsTrainVib)
	{
		// �÷��̾� �ѹ��� ���� ���� ȿ�� �ֱ�
		InputManager::GetInstance()->SetControllerVib(m_PlayerNumber, MinVibrationValue, MinVibrationValue);
		m_IsTrainVib = false;
	}
}

void PlayerObject::OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{
	ItemBoxObject* itemBoxObj = dynamic_cast<ItemBoxObject*>(otherCollision->GetOwner());
	MoneyObject* moneyObj = dynamic_cast<MoneyObject*>(otherCollision->GetOwner());

	// ItemBox�� �浹 (�÷��̾��� �� �ݶ��̴����� �ش�)
	if (itemBoxObj && thisCollision == GetBoxColliderFull())
	{
		// ������ �ڽ� Ÿ��
		const eItemBoxType itemBoxType = itemBoxObj->GetItemBoxType();

		// �ش� �÷��̾ ������ �ڽ� Ÿ�Կ� ���� �������� �̹� ������ ���� �ʾ� �������� �������� ���� �� �ִ� ���
		if (GetRandomItem(itemBoxType))
		{
			if (m_PlayerNumber == 0)
				SoundManager::GetInstance()->PlayMusic(eSoundList::ItemBoxEffect, eSoundChannel::Player1State, 1.f);
			else if (m_PlayerNumber == 1)																	   
				SoundManager::GetInstance()->PlayMusic(eSoundList::ItemBoxEffect, eSoundChannel::Player2State, 1.f);
			else if (m_PlayerNumber == 2)																	   
				SoundManager::GetInstance()->PlayMusic(eSoundList::ItemBoxEffect, eSoundChannel::Player3State, 1.f);
			else if (m_PlayerNumber == 3)																	   
				SoundManager::GetInstance()->PlayMusic(eSoundList::ItemBoxEffect, eSoundChannel::Player4State, 1.f);

			// ������ �ڽ� ����
			EventManager::GetInstance()->SendEvent(eEventType::DeleteGameObject, GROUP_TYPE::ITEMBOX, itemBoxObj);

			if (itemBoxType == eItemBoxType::THROW)
			{
				EventManager::GetInstance()->SendEvent(eEventType::ItemVisible, m_PlayerNumber, eItemBoxType::THROW, m_OwnedItem[static_cast<UINT>(eItemBoxType::THROW)]);
				EventManager::GetInstance()->SendEvent(eEventType::DeleteThrow);
			}
			else if (itemBoxType == eItemBoxType::REINFORCED)
			{
				EventManager::GetInstance()->SendEvent(eEventType::ItemVisible, m_PlayerNumber, eItemBoxType::REINFORCED, m_OwnedItem[static_cast<UINT>(eItemBoxType::REINFORCED)]);
				EventManager::GetInstance()->SendEvent(eEventType::DeleteReinforced);
			}
			else if (itemBoxType == eItemBoxType::INSTALLATION)
			{
				EventManager::GetInstance()->SendEvent(eEventType::ItemVisible, m_PlayerNumber, eItemBoxType::INSTALLATION, m_OwnedItem[static_cast<UINT>(eItemBoxType::INSTALLATION)]);
				EventManager::GetInstance()->SendEvent(eEventType::DeleteInstallation);
			}

			// Exit ó�� ���ֱ�
			thisCollision->TurnOff_IsTrigger(otherCollision);

			iter->second = false;

			otherCollision->OnTriggerExit(thisCollision);
			thisCollision->OnTriggerExit(otherCollision);
		}
	}
	// MoneyObject�� �浹
	else if (moneyObj
		&& (moneyObj->GetMoneyType() == eMoneyType::DROPED_COLLISION
			|| moneyObj->GetMoneyType() == eMoneyType::SPAWNED)
		&& m_Money < 10 && thisCollision == GetBoxColliderFull())
	{
		EventManager::GetInstance()->SendEvent(eEventType::DeleteMoney);
		EventManager::GetInstance()->SendEvent(eEventType::DeleteGameObject, GROUP_TYPE::MONEY, otherCollision->GetOwner());

		// Exit ó�� ���ֱ�
		thisCollision->TurnOff_IsTrigger(otherCollision);

		iter->second = false;

		otherCollision->SetCollisionType(COLLISION_TYPE::NONE);
		otherCollision->OnTriggerExit(thisCollision);
		thisCollision->OnTriggerExit(otherCollision);

		m_Money++;

		if (m_PlayerNumber == 0)
			SoundManager::GetInstance()->PlayMusic(eSoundList::PickUpEffect, eSoundChannel::Player1State, 0.9f);
		else if (m_PlayerNumber == 1)																			  
			SoundManager::GetInstance()->PlayMusic(eSoundList::PickUpEffect, eSoundChannel::Player2State, 0.9f);
		else if (m_PlayerNumber == 2)																			  
			SoundManager::GetInstance()->PlayMusic(eSoundList::PickUpEffect, eSoundChannel::Player3State, 0.9f);
		else if (m_PlayerNumber == 3)																			  
			SoundManager::GetInstance()->PlayMusic(eSoundList::PickUpEffect, eSoundChannel::Player4State, 0.9f);

		// ���� ���
		this->SetVibration(0.15f, MinVibrationValue, MaxVibrationValue);

		m_pMovementComponent->SetOriginalVelocityLimit(0.95f);
		SetBundleSize();
	}
}

void PlayerObject::OnTriggerStay(Collider2D* otherCollision)
{

}

void PlayerObject::OnTriggerExit(Collider2D* otherCollision)
{

}

void PlayerObject::HandleEvent(Event* event)
{
	if(m_PlayerNumber == 0)
	{
		// ����ȭ ����
		if (event->eventID == eEventType::Player1TransparentStart)
		{
			m_pAnimComponent->SetVisible(false);
			return;
		}
		// ����ȭ ��
		if (event->eventID == eEventType::Player1TransparentEnd)
		{
			m_pAnimComponent->SetVisible(true);
			return;
		}

		if (event->eventID == eEventType::P1KeepAnimation)
		{
			m_pAnimComponent->KeepAnimation(event->animationInfo.animationName, event->animationInfo.flip);
			return;
		}
		if (event->eventID == eEventType::P1ChangeAnimation)
		{
			m_pAnimComponent->ChangeAnimation(event->animationInfo.animationName, event->animationInfo.flip);
			return;
		}
		if (event->eventID == eEventType::P1SetDefaultAnimation)
		{
			m_pAnimComponent->SetAnimationName(event->animationInfo.animationName);
			return;
		}
	}
	if (m_PlayerNumber == 1)
	{
		// ����ȭ ����
		if (event->eventID == eEventType::Player2TransparentStart)
		{
			m_pAnimComponent->SetVisible(false);
			return;
		}
		// ����ȭ ��
		if (event->eventID == eEventType::Player2TransparentEnd)
		{
			m_pAnimComponent->SetVisible(true);
			return;
		}
		// ����ȭ ����
		if (event->eventID == eEventType::Player2TransparentStart)
		{
			m_pAnimComponent->SetVisible(false);
			return;
		}
		// ����ȭ ��
		if (event->eventID == eEventType::Player2TransparentEnd)
		{
			m_pAnimComponent->SetVisible(true);
			return;
		}

		if (event->eventID == eEventType::P2KeepAnimation)
		{
			m_pAnimComponent->KeepAnimation(event->animationInfo.animationName, event->animationInfo.flip);
			return;
		}
		if (event->eventID == eEventType::P2ChangeAnimation)
		{
			m_pAnimComponent->ChangeAnimation(event->animationInfo.animationName, event->animationInfo.flip);
			return;
		}
		if (event->eventID == eEventType::P2SetDefaultAnimation)
		{
			m_pAnimComponent->SetAnimationName(event->animationInfo.animationName);
			return;
		}
	}
	if (m_PlayerNumber == 2)
	{
		// ����ȭ ����
		if (event->eventID == eEventType::Player3TransparentStart)
		{
			m_pAnimComponent->SetVisible(false);
			return;
		}
		// ����ȭ ��
		if (event->eventID == eEventType::Player3TransparentEnd)
		{
			m_pAnimComponent->SetVisible(true);
			return;
		}
		if (event->eventID == eEventType::P3KeepAnimation)
		{
			m_pAnimComponent->KeepAnimation(event->animationInfo.animationName, event->animationInfo.flip);
			return;
		}
		if (event->eventID == eEventType::P3ChangeAnimation)
		{
			m_pAnimComponent->ChangeAnimation(event->animationInfo.animationName, event->animationInfo.flip);
			return;
		}
		if (event->eventID == eEventType::P3SetDefaultAnimation)
		{
			m_pAnimComponent->SetAnimationName(event->animationInfo.animationName);
			return;
		}
	}
	if (m_PlayerNumber == 3)
	{
		// ����ȭ ����
		if (event->eventID == eEventType::Player4TransparentStart)
		{
			m_pAnimComponent->SetVisible(false);
			return;
		}
		// ����ȭ ��
		if (event->eventID == eEventType::Player4TransparentEnd)
		{
			m_pAnimComponent->SetVisible(true);
			return;
		}
		if (event->eventID == eEventType::P4KeepAnimation)
		{
			m_pAnimComponent->KeepAnimation(event->animationInfo.animationName, event->animationInfo.flip);
			return;
		}
		if (event->eventID == eEventType::P4ChangeAnimation)
		{
			m_pAnimComponent->ChangeAnimation(event->animationInfo.animationName, event->animationInfo.flip);
			return;
		}
		if (event->eventID == eEventType::P4SetDefaultAnimation)
		{
			m_pAnimComponent->SetAnimationName(event->animationInfo.animationName);
			return;
		}
	}
}

void PlayerObject::AttachFSM(FSM* fsm)
{
	m_pFSMComponent->AttachFSM(fsm);
}

void PlayerObject::SendHPToFSM()
{
	switch (m_PlayerNumber)
	{
	case 0 :
		EventManager::GetInstance()->SendEvent(eEventType::P1TransperHP, m_HP);
		return;
	case 1:
		EventManager::GetInstance()->SendEvent(eEventType::P2TransperHP, m_HP);
		return;
	case 2:
		EventManager::GetInstance()->SendEvent(eEventType::P3TransperHP, m_HP);
		return;
	case 3:
		EventManager::GetInstance()->SendEvent(eEventType::P4TransperHP, m_HP);
		return;
	}
}

void PlayerObject::GetDamaged(int damage, int money)
{
	m_HP -= damage;

	int dropCnt;
	if(m_Money - money < 0)
	{
		dropCnt = m_Money;
		m_Money = 0;
	}
	else
	{
		dropCnt = money;
		m_Money -= money;
	}
	m_pMovementComponent->SetOriginalVelocityLimit(1.f + (dropCnt * 0.05f));

	// std::cout << "dropMoney : " << dropCnt << std::endl;
	// std::cout << "playerMoney : " << m_Money << std::endl;

	while(dropCnt)
	{
		MoneyObject* dropedMoney = m_pOwnerWorld->CreateGameObject<MoneyObject>(L"DropMoney", GROUP_TYPE::MONEY);
		dropedMoney->SetMoneyType(eMoneyType::DROPED_UNCOLLISION);

		dropedMoney->Initialize();

		dropedMoney->SetGameObjectLocation(GetGameObjectLocation().x, GetGameObjectLocation().y);

		--dropCnt;
	}

	m_playerTimer = 0.f;
}

void PlayerObject::InitAnimationAsset()
{
	// Plyaer FSM
	std::vector<framework::FRAME_INFO> frame_IdleBack;
	frame_IdleBack.emplace_back(framework::FRAME_INFO({ 0, 0, 80, 110 }, 0.1f));
	frame_IdleBack.emplace_back(framework::FRAME_INFO({ 80, 0, 160, 110 }, 0.1f));
	frame_IdleBack.emplace_back(framework::FRAME_INFO({ 160, 0, 240, 110 }, 0.1f));
	frame_IdleBack.emplace_back(framework::FRAME_INFO({ 240, 0, 320, 110 }, 0.1f));

	std::vector<framework::FRAME_INFO> frame_IdleFront;
	frame_IdleFront.emplace_back(framework::FRAME_INFO({ 0, 110, 80, 220 }, 0.1f));
	frame_IdleFront.emplace_back(framework::FRAME_INFO({ 80, 110, 160, 220 }, 0.1f));
	frame_IdleFront.emplace_back(framework::FRAME_INFO({ 160, 110, 240, 220 }, 0.1f));
	frame_IdleFront.emplace_back(framework::FRAME_INFO({ 240, 110, 320, 220 }, 0.1f));

	std::vector<framework::FRAME_INFO> frame_IdleSide;
	frame_IdleSide.emplace_back(framework::FRAME_INFO({ 0, 220, 80, 330 }, 0.1f));
	frame_IdleSide.emplace_back(framework::FRAME_INFO({ 80, 220, 160, 330 }, 0.1f));
	frame_IdleSide.emplace_back(framework::FRAME_INFO({ 160, 220, 240, 330 }, 0.1f));
	frame_IdleSide.emplace_back(framework::FRAME_INFO({ 240, 220, 320, 330 }, 0.1f));

	std::vector<framework::FRAME_INFO> frame_MoveBack;
	frame_MoveBack.emplace_back(framework::FRAME_INFO({ 0, 330, 80, 440 }, 0.1f));
	frame_MoveBack.emplace_back(framework::FRAME_INFO({ 80, 330, 160, 440 }, 0.1f));
	frame_MoveBack.emplace_back(framework::FRAME_INFO({ 160, 330, 240, 440 }, 0.1f));
	frame_MoveBack.emplace_back(framework::FRAME_INFO({ 240, 330, 320, 440 }, 0.1f));
	frame_MoveBack.emplace_back(framework::FRAME_INFO({ 320, 330, 400, 440 }, 0.1f));
	frame_MoveBack.emplace_back(framework::FRAME_INFO({ 400, 330, 480, 440 }, 0.1f));
	frame_MoveBack.emplace_back(framework::FRAME_INFO({ 480, 330, 560, 440 }, 0.1f));
	frame_MoveBack.emplace_back(framework::FRAME_INFO({ 560, 330, 640, 440 }, 0.1f));

	std::vector<framework::FRAME_INFO> frame_MoveFront;
	frame_MoveFront.emplace_back(framework::FRAME_INFO({ 0, 440, 80, 550 }, 0.1f));
	frame_MoveFront.emplace_back(framework::FRAME_INFO({ 80, 440, 160, 550 }, 0.1f));
	frame_MoveFront.emplace_back(framework::FRAME_INFO({ 160, 440, 240, 550 }, 0.1f));
	frame_MoveFront.emplace_back(framework::FRAME_INFO({ 240, 440, 320, 550 }, 0.1f));
	frame_MoveFront.emplace_back(framework::FRAME_INFO({ 320, 440, 400, 550 }, 0.1f));
	frame_MoveFront.emplace_back(framework::FRAME_INFO({ 400, 440, 480, 550 }, 0.1f));
	frame_MoveFront.emplace_back(framework::FRAME_INFO({ 480, 440, 560, 550 }, 0.1f));
	frame_MoveFront.emplace_back(framework::FRAME_INFO({ 560, 440, 640, 550 }, 0.1f));

	std::vector<framework::FRAME_INFO> frame_MoveSide;
	frame_MoveSide.emplace_back(framework::FRAME_INFO({ 0,550, 80, 660 }, 0.1f));
	frame_MoveSide.emplace_back(framework::FRAME_INFO({ 80,550, 160, 660 }, 0.1f));
	frame_MoveSide.emplace_back(framework::FRAME_INFO({ 160,550, 240, 660 }, 0.1f));
	frame_MoveSide.emplace_back(framework::FRAME_INFO({ 240,550, 320, 660 }, 0.1f));
	frame_MoveSide.emplace_back(framework::FRAME_INFO({ 320,550, 400, 660 }, 0.1f));
	frame_MoveSide.emplace_back(framework::FRAME_INFO({ 400,550, 480, 660 }, 0.1f));
	frame_MoveSide.emplace_back(framework::FRAME_INFO({ 480,550, 560, 660 }, 0.1f));
	frame_MoveSide.emplace_back(framework::FRAME_INFO({ 560,550, 640, 660 }, 0.1f));

	std::vector<framework::FRAME_INFO> frame_Stun;
	frame_Stun.emplace_back(framework::FRAME_INFO({ 0,660, 80, 780 }, 0.1f));
	frame_Stun.emplace_back(framework::FRAME_INFO({ 80,660, 160, 780 }, 0.1f));
	frame_Stun.emplace_back(framework::FRAME_INFO({ 160,660, 240, 780 }, 0.1f));
	frame_Stun.emplace_back(framework::FRAME_INFO({ 240,660, 320, 780 }, 0.1f));

	if (m_name == L"Player1")
	{
		m_pAnimComponent->SetAnimationAsset(L"../Resource/red_racoon_sheet.png", L"IDLE_BACK", frame_IdleBack);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/red_racoon_sheet.png", L"IDLE_FRONT", frame_IdleFront);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/red_racoon_sheet.png", L"IDLE_SIDE", frame_IdleSide);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/red_racoon_sheet.png", L"MOVE_BACK", frame_MoveBack);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/red_racoon_sheet.png", L"MOVE_FRONT", frame_MoveFront);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/red_racoon_sheet.png", L"MOVE_SIDE", frame_MoveSide);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/red_racoon_sheet.png", L"STUN", frame_Stun);
	}
	else if (m_name == L"Player2")
	{
		m_pAnimComponent->SetAnimationAsset(L"../Resource/blue_racoon_sheet.png", L"IDLE_BACK", frame_IdleBack);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/blue_racoon_sheet.png", L"IDLE_FRONT", frame_IdleFront);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/blue_racoon_sheet.png", L"IDLE_SIDE", frame_IdleSide);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/blue_racoon_sheet.png", L"MOVE_BACK", frame_MoveBack);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/blue_racoon_sheet.png", L"MOVE_FRONT", frame_MoveFront);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/blue_racoon_sheet.png", L"MOVE_SIDE", frame_MoveSide);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/blue_racoon_sheet.png", L"STUN", frame_Stun);
	}
	else if (m_name == L"Player3")
	{
		m_pAnimComponent->SetAnimationAsset(L"../Resource/yellow_racoon_sheet.png", L"IDLE_BACK", frame_IdleBack);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/yellow_racoon_sheet.png", L"IDLE_FRONT", frame_IdleFront);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/yellow_racoon_sheet.png", L"IDLE_SIDE", frame_IdleSide);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/yellow_racoon_sheet.png", L"MOVE_BACK", frame_MoveBack);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/yellow_racoon_sheet.png", L"MOVE_FRONT", frame_MoveFront);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/yellow_racoon_sheet.png", L"MOVE_SIDE", frame_MoveSide);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/yellow_racoon_sheet.png", L"STUN", frame_Stun);
	}
	else if (m_name == L"Player4")
	{
		m_pAnimComponent->SetAnimationAsset(L"../Resource/green_racoon_sheet.png", L"IDLE_BACK", frame_IdleBack);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/green_racoon_sheet.png", L"IDLE_FRONT", frame_IdleFront);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/green_racoon_sheet.png", L"IDLE_SIDE", frame_IdleSide);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/green_racoon_sheet.png", L"MOVE_BACK", frame_MoveBack);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/green_racoon_sheet.png", L"MOVE_FRONT", frame_MoveFront);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/green_racoon_sheet.png", L"MOVE_SIDE", frame_MoveSide);
		m_pAnimComponent->SetAnimationAsset(L"../Resource/green_racoon_sheet.png", L"STUN", frame_Stun);
	}

	// Slow Effect
	std::vector<framework::FRAME_INFO> effectFrame;
	effectFrame.emplace_back(framework::FRAME_INFO{ 0, 0, 200, 260, 0.22f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 200, 0, 200, 260, 0.22f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 400, 0, 200, 260, 0.22f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 600, 0, 200, 260, 0.22f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 800, 0, 200, 260, 0.22f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1000, 0, 200, 260, 0.22f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1200, 0, 200, 260, 0.22f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1400, 0, 200, 260, 0.22f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1600, 0, 200, 260, 0.22f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1800, 0, 200, 260, 0.22f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2000, 0, 200, 260, 0.22f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2200, 0, 200, 260, 0.22f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2400, 0, 200, 260, 0.22f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2600, 0, 200, 260, 0.22f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2800, 0, 200, 260, 0.22f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 3000, 0, 200, 260, 0.22f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 3200, 0, 200, 260, 0.22f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 0, 0, 0, 0, 0.1f });
	m_pSlowAnimation->SetAnimationAsset(L"../Resource/Effect_Slow.png", L"SlowEffect", effectFrame);
	m_pSlowAnimation->SetAnimationName(L"SlowEffect");

	// Blood Effect
	effectFrame.clear();
	effectFrame.emplace_back(framework::FRAME_INFO{ 0, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 160, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 320, 0, 10, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 480, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 640, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 800, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 960, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1120, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1280, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1440, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1600, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1760, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1920, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2080, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2240, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2400, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2560, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2720, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2880, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 3040, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 3200, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 3360, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 3520, 0, 160, 220, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 0, 0, 0, 0, 0.1f });
	m_pBloodAnimation->SetAnimationAsset(L"../Resource/Effect_Blood.png", L"BloodEffect", effectFrame);
	m_pBloodAnimation->SetAnimationName(L"BloodEffect");

	// Negative Effect
	effectFrame.clear();
	effectFrame.emplace_back(framework::FRAME_INFO{ 0, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 160, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 320, 0, 10, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 480, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 640, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 800, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 960, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1120, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1280, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1440, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1600, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1760, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1920, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2080, 0, 160,50 , 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2240, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2400, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2560, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2720, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 2880, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 3040, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 3200, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 3360, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 3520, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 3680, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 3840, 0, 160, 50, 0.2f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 0, 0, 0, 0, 0.1f });
	m_pNegativeAnimation->SetAnimationAsset(L"../Resource/Effect_Negative.png", L"NegativeEffect", effectFrame);
	m_pNegativeAnimation->SetAnimationName(L"NegativeEffect");
}

void PlayerObject::InitTextureAsset()
{
	if (m_name == L"Player1")
	{
		m_pShadowTexture->SetTextureAsset(L"../Resource/red_0.png", L"");
	}
	else if (m_name == L"Player2")
	{
		m_pShadowTexture->SetTextureAsset(L"../Resource/blue_0.png", L"");
	}
	else if (m_name == L"Player3")
	{
		m_pShadowTexture->SetTextureAsset(L"../Resource/yellow_0.png", L"");
	}
	else if (m_name == L"Player4")
	{
		m_pShadowTexture->SetTextureAsset(L"../Resource/green_0.png", L"");
	}
}

// OnTriggerEnter���� ��� Object�� ItemBox���� Ȯ���ϰ� ȣ��
bool PlayerObject::GetRandomItem(eItemBoxType itemBoxType)
{
	TestWorld* testWorld = dynamic_cast<TestWorld*>(m_pOwnerWorld);
	InGameWorld* inGameWorld = dynamic_cast<InGameWorld*>(m_pOwnerWorld);

	DirectionComponent* directionComponent = this->GetComponent<DirectionComponent>();

	if(testWorld)
	{
		if (m_OwnedItem[static_cast<UINT>(itemBoxType)] == eItemType::NOTHING)
		{
			m_OwnedItem[static_cast<UINT>(itemBoxType)] = testWorld->GetRandomItem(itemBoxType);
			return true;
		}
		else
		{
			return false;
		}
	}
	if(inGameWorld)
	{
		if (m_OwnedItem[static_cast<UINT>(itemBoxType)] == eItemType::NOTHING)
		{
			m_OwnedItem[static_cast<UINT>(itemBoxType)] = inGameWorld->GetRandomItem(itemBoxType);

			// ȹ���� ������ Ÿ��
			const eItemType itemType = m_OwnedItem[static_cast<UINT>(itemBoxType)];

			switch(itemType)
			{
			case eItemType::PunchItem:
				directionComponent->SetTextureAsset(L"../Resource/punch.png", L"Punch");
				break;
			case eItemType::SnowItem:
				directionComponent->SetTextureAsset(L"../Resource/Snowball_2.png", L"SnowBall");
				break;
			case eItemType::StungunItem:
				directionComponent->SetTextureAsset(L"../Resource/stunGun.png", L"StunGun");
				break;
			case eItemType::BottleItem:
				directionComponent->SetTextureAsset(L"../Resource/glassBottle.png", L"GlassBottle");
				break;
			default:
				break;
			}

			// m_PlayerUsageCnt : �÷��̾ �� �������� ��������� �󸶳� �� �� �� �ִ��� / m_ItemUsageCnt : ������ ��ü ����
			m_PlayerUsageCnt[static_cast<UINT>(itemBoxType)] = m_ItemUsageCnt[static_cast<UINT>(m_OwnedItem[static_cast<UINT>(itemBoxType)])];

			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

void PlayerObject::CreateInstance(eItemType itemType)
{
	m_pCreatedItem = nullptr;

	switch(itemType)
	{
	// THROW
	case eItemType::PunchItem :
		m_pCreatedItem = m_pOwnerWorld->CreateGameObject<PunchItem>(L"PunchItem", GROUP_TYPE::ITEM);
		m_pCreatedItem->SetItemBoxType(eItemBoxType::THROW);
		break;
	case eItemType::SnowItem :
		m_pCreatedItem = m_pOwnerWorld->CreateGameObject<SnowItem>(L"SnowItem", GROUP_TYPE::ITEM);
		m_pCreatedItem->SetItemBoxType(eItemBoxType::THROW);
		break;
	case eItemType::StungunItem:
		m_pCreatedItem = m_pOwnerWorld->CreateGameObject<StungunItem>(L"StunGunItem", GROUP_TYPE::ITEM);
		m_pCreatedItem->SetItemBoxType(eItemBoxType::THROW);
		break;
	case eItemType::BottleItem:
		m_pCreatedItem = m_pOwnerWorld->CreateGameObject<BottleItem>(L"BottleItem", GROUP_TYPE::ITEM);
		m_pCreatedItem->SetItemBoxType(eItemBoxType::THROW);
		break;

	// INSTALLATION
	case eItemType::SnareItem:
		m_pCreatedItem = m_pOwnerWorld->CreateGameObject<SnareItem>(L"SnareItem", GROUP_TYPE::ITEM);
		m_pCreatedItem->SetItemBoxType(eItemBoxType::INSTALLATION);
		break;
	case eItemType::TrapItem:
		m_pCreatedItem = m_pOwnerWorld->CreateGameObject<TrapItem>(L"TrapItem", GROUP_TYPE::ITEM);
		m_pCreatedItem->SetItemBoxType(eItemBoxType::INSTALLATION);
		break;

	// REINFORCED
	case eItemType::ShoeItem:
		m_pCreatedItem = m_pOwnerWorld->CreateGameObject<ShoeItem>(L"ShoeItem", GROUP_TYPE::ITEM);
		m_pCreatedItem->SetItemBoxType(eItemBoxType::REINFORCED);
		break;
	case eItemType::WaveItem:
		m_pCreatedItem = m_pOwnerWorld->CreateGameObject<WaveItem>(L"WaveItem", GROUP_TYPE::ITEM);
		m_pCreatedItem->SetItemBoxType(eItemBoxType::REINFORCED);
		break;
	case eItemType::TransparencyItem:
		m_pCreatedItem = m_pOwnerWorld->CreateGameObject<TransparencyItem>(L"TransparencyItem", GROUP_TYPE::ITEM);
		m_pCreatedItem->SetItemBoxType(eItemBoxType::REINFORCED);
		break;

	default :
		return;
	}

	// ����� �������� ����ü �������� ��� ���� �ʿ�
	if (itemType == eItemType::PunchItem || itemType == eItemType::SnowItem || itemType == eItemType::StungunItem || itemType == eItemType::BottleItem)
	{
		// ����ü �������� ��� ����� ��� Clear �ؽ��ķ� ����
		if(m_PlayerUsageCnt[static_cast<UINT>(eItemBoxType::THROW)] <= 1)
		{
			// std::cout << m_PlayerUsageCnt << std::endl;
			DirectionComponent* directionComponent = this->GetComponent<DirectionComponent>();
			directionComponent->SetTextureAsset(L"../Resource/clear.png", L"Clear");
		}

		// ItemObject�� ProjectileObject�� ĳ����
		ProjectileObject* projectile = dynamic_cast<ProjectileObject*>(m_pCreatedItem);

		// �������� ������ ����
		projectile->SetOwnerPlayer(this);

		// �ӵ� ���� (����ü�� �⺻ �ӵ��� �̿��ص� �ȴ�)
		projectile->SetSpeed(1000.f);

		// �ٶ󺸴� ���⿡ ���� ȸ�� ����
		const framework::Vector2D tempLookDir = GetComponent<PlayerMovement>()->GetLookDir();
		float theta = atan(tempLookDir.y / tempLookDir.x); // ��ũź��Ʈ �Լ��� ����� theta�� ����
		theta = theta * 180.f / 3.14159265f; // ������ ���� ��ȯ
		projectile->SetRotation(theta);

		// ���� ����
		projectile->SetDirection(tempLookDir);

		// ����ü �������� ���� ��ġ ����
		const framework::Vector2D pos = GetRootComponent()->GetRelativeLocation();
		constexpr float distance = 75.f;
		const framework::Vector2D distVec = tempLookDir * distance;
		projectile->SetPosition(pos + distVec);

		// ����ü �����ۿ� ������Ʈ ���� �ʱ�ȭ
		projectile->Initialize();

		// ����ü �������� �̹��� ����
		if (tempLookDir.x < 0)
			dynamic_cast<TextureComponent*>(projectile->GetRootComponent())->SetFlipX(true);
		else if (tempLookDir.x >= 0)
			dynamic_cast<TextureComponent*>(projectile->GetRootComponent())->SetFlipX(false);

		return;
	}

	m_pCreatedItem->SetOwnerObj(this);
	m_pCreatedItem->SetOwnerPlayer(this);
	m_pCreatedItem->Initialize();
	//m_pCreatedItem->SetGameObjectLocation(GetGameObjectLocation().x, GetGameObjectLocation().y);
}

void PlayerObject::UseThrowItem()
{
	if (m_PlayerNumber == 0)
		SoundManager::GetInstance()->PlayMusic(eSoundList::ThrowEffect, eSoundChannel::Player1UseItem, 1.f);
	else if (m_PlayerNumber == 1)																	   
		SoundManager::GetInstance()->PlayMusic(eSoundList::ThrowEffect, eSoundChannel::Player2UseItem, 1.f);
	else if (m_PlayerNumber == 2)																	   
		SoundManager::GetInstance()->PlayMusic(eSoundList::ThrowEffect, eSoundChannel::Player3UseItem, 1.f);
	else if (m_PlayerNumber == 3)																	   
		SoundManager::GetInstance()->PlayMusic(eSoundList::ThrowEffect, eSoundChannel::Player4UseItem, 1.f);

	// ���� ���
	this->SetVibration(0.15f, MinVibrationValue, MaxVibrationValue);

	m_IsItemUsable = false;
	CreateInstance(m_OwnedItem[static_cast<UINT>(eItemBoxType::THROW)]);
	--m_PlayerUsageCnt[static_cast<UINT>(eItemBoxType::THROW)];

	if(m_PlayerUsageCnt[static_cast<UINT>(eItemBoxType::THROW)] == 0)
	{
		EventManager::GetInstance()->SendEvent(eEventType::ItemInVisible, m_PlayerNumber, eItemBoxType::THROW, m_OwnedItem[static_cast<UINT>(eItemBoxType::THROW)]);

		m_OwnedItem[static_cast<UINT>(eItemBoxType::THROW)] = eItemType::NOTHING;
	}
	// printf("UseThrowItem");
}

void PlayerObject::UseInstallationItem()
{
	if (m_PlayerNumber == 0)
		SoundManager::GetInstance()->PlayMusic(eSoundList::SetupEffect, eSoundChannel::Player1UseItem, 0.8f);
	else if (m_PlayerNumber == 1)																			   
		SoundManager::GetInstance()->PlayMusic(eSoundList::SetupEffect, eSoundChannel::Player2UseItem, 0.8f);
	else if (m_PlayerNumber == 2)																			   
		SoundManager::GetInstance()->PlayMusic(eSoundList::SetupEffect, eSoundChannel::Player3UseItem, 0.8f);
	else if (m_PlayerNumber == 3)																			   
		SoundManager::GetInstance()->PlayMusic(eSoundList::SetupEffect, eSoundChannel::Player4UseItem, 0.8f);

	this->SetVibration(0.15f, MinVibrationValue, MaxVibrationValue);

	m_IsItemUsable = false;
	CreateInstance(m_OwnedItem[static_cast<UINT>(eItemBoxType::INSTALLATION)]);
	--m_PlayerUsageCnt[static_cast<UINT>(eItemBoxType::INSTALLATION)];
	if (m_PlayerUsageCnt[static_cast<UINT>(eItemBoxType::INSTALLATION)] == 0)
	{
		EventManager::GetInstance()->SendEvent(eEventType::ItemInVisible, m_PlayerNumber, eItemBoxType::INSTALLATION, m_OwnedItem[static_cast<UINT>(eItemBoxType::INSTALLATION)]);
		m_OwnedItem[static_cast<UINT>(eItemBoxType::INSTALLATION)] = eItemType::NOTHING;
	}
	// printf("UseInstallationItem");
}

void PlayerObject::UseReinforcedItem()
{
	m_IsItemUsable = false;
	CreateInstance(m_OwnedItem[static_cast<UINT>(eItemBoxType::REINFORCED)]);
	--m_PlayerUsageCnt[static_cast<UINT>(eItemBoxType::REINFORCED)];
	// printf("UseReinforcedItem");
}

void PlayerObject::SetReinforcedNothing()
{
	EventManager::GetInstance()->SendEvent(eEventType::ItemInVisible, m_PlayerNumber, eItemBoxType::REINFORCED, m_OwnedItem[static_cast<UINT>(eItemBoxType::REINFORCED)]);
	m_OwnedItem[static_cast<UINT>(eItemBoxType::REINFORCED)] = eItemType::NOTHING;
}

void PlayerObject::ItemCoolTimer(float deltaTime)
{
	// ������ ��Ÿ�� Ÿ�̸�
	if (!m_IsItemUsable)
	{
		m_itemTimer += deltaTime;

		if (m_itemTimer >= m_itemCoolTime)
		{
			m_IsItemUsable = true;
			m_itemTimer = 0.f;
		}
	}
}

bool PlayerObject::PlayerGetDamagedTimer(float deltaTime)
{
	// 2�� �������� �� ��������
	m_playerTimer += deltaTime;

	if (m_playerTimer > 2.f)
	{
		m_playerTimer = 0.f;
		return false;
	}
	return true;
}

void PlayerObject::SetBundleSize()
{
	if (m_Money >= 5 && m_Money < 10)
	{
		m_bundleX = 45.f;
		m_bundleY = -10.f;
		m_pBundleTexture->SetTextureAsset(L"../Resource/bundle_M.png", L"");
	}
	else if (m_Money >= 10)
	{
		m_bundleX = 58.f;
		m_bundleY = -18.f;
		m_pBundleTexture->SetTextureAsset(L"../Resource/bundle_L.png", L"");
	}
	else if (m_Money >= 1 && m_Money < 5)
	{
		m_bundleX = 30.f;
		m_bundleY = -8.f;
		m_pBundleTexture->SetTextureAsset(L"../Resource/bundle_S.png", L"");
	}
}

void PlayerObject::CalculateDeadVelocity()
{
	// ���ư��� �ӵ� ���� ���
	const framework::Vector2D playerPosition = GetRootComponent()->GetWorldLocation();
	m_MoveDirToSpawn = m_SpawnPosition - playerPosition;
	m_MoveDirToSpawn.Normalize();
}

void PlayerObject::TurnOffPlayerMovement()
{
	// Off PlayerMovement
	PlayerMovement* playerMovement = this->GetComponent<PlayerMovement>();
	if (playerMovement)
		playerMovement->SetActive(false);
}

void PlayerObject::TurnOnPlayerMovement()
{
	// Off PlayerMovement
	PlayerMovement* playerMovement = this->GetComponent<PlayerMovement>();
	if (playerMovement)
		playerMovement->SetActive(true);
}

void PlayerObject::ResetDeadMove()
{
	m_DeadSpeed = 1000.f;
	m_DeadRotation = 2000.f;
	m_MoveDirToSpawn = { 0.f, 0.f };
}

bool PlayerObject::IsOutOfRange()
{
	// ȭ�� ������ �Ѿ

	// ����
	if (this->GetRootComponent()->GetWorldLocation().x > ScreenWidth - 70.f || this->GetRootComponent()->GetWorldLocation().x < 0 + 70.f)
	{
		return true;
	}

	// ���Ʒ�
	if (this->GetRootComponent()->GetWorldLocation().y >= ScreenHeight - 50.f)
	{
		this->SetGameObjectLocation(this->GetRootComponent()->GetWorldLocation().x, ScreenHeight - 50.f);
	}
	if (this->GetRootComponent()->GetWorldLocation().y <= 0)
	{
		this->SetGameObjectLocation(this->GetRootComponent()->GetWorldLocation().x, 0.f);
	}

	// 0, 0�� ��������� ����������
	if (this->GetRootComponent()->GetWorldLocation().x <= 80.f)
	{
		if (this->GetRootComponent()->GetWorldLocation().y <= 80.f)
		{
			this->SetGameObjectLocation(this->GetRootComponent()->GetWorldLocation().x, 80.f);
		}
	}
	if (this->GetRootComponent()->GetWorldLocation().y <= 80.f)
	{
		if (this->GetRootComponent()->GetWorldLocation().x <= 80.f)
		{
			this->SetGameObjectLocation(80.f, this->GetRootComponent()->GetWorldLocation().y);
		}
	}

	return false;
}

void PlayerObject::TurnToStun()
{
	m_HP = 0;
	m_bStun = true;

	if (!m_stunPlayTime)
	{
		if (m_PlayerNumber == 0)
			SoundManager::GetInstance()->PlayMusic(eSoundList::StunEffect, eSoundChannel::Player1State, 1.f);
		else if (m_PlayerNumber == 1)																	
			SoundManager::GetInstance()->PlayMusic(eSoundList::StunEffect, eSoundChannel::Player2State, 1.f);
		else if (m_PlayerNumber == 2)																	
			SoundManager::GetInstance()->PlayMusic(eSoundList::StunEffect, eSoundChannel::Player3State, 1.f);
		else if (m_PlayerNumber == 3)																	
			SoundManager::GetInstance()->PlayMusic(eSoundList::StunEffect, eSoundChannel::Player4State, 1.f);

		m_stunPlayTime = true;
	}

	// Off All Collider
	//std::vector<Collider2D*> thiscolliders = this->GetComponents<Collider2D>();
	//for (auto thisCollider : thiscolliders)
	//{
	//	thisCollider->SetCollisionType(COLLISION_TYPE::NONE);
	//}

	// 2�� ���� ���� ���
	this->SetVibration(2.f, MaxVibrationValue / 4, MaxVibrationValue / 2);

	// �÷��̾� ������ ����
	TurnOffPlayerMovement();

	// �÷��̾� ����� ���ư��� �ӵ� ���
	CalculateDeadVelocity();
}

void PlayerObject::MoveToSpawnImmediately()
{
	this->SetGameObjectLocation(m_SpawnPosition.x, m_SpawnPosition.y);

	// �̵��ӵ� �ʱ�ȭ
	m_pMovementComponent->ResetVelocityLimit();
	// ���ӵ� �ʱ�ȭ
	m_pMovementComponent->ResetExternalVelocity();

	m_pMovementComponent->SetOriginalVelocityLimit(1.f - (static_cast<float>(m_Money) * 0.05f));
}

void PlayerObject::MoveToSpawnWithRotation(float deltaTime)
{
	if(framework::Vector2D::Distance(GetRootComponent()->GetWorldLocation(), m_SpawnPosition) > 10.f)
	{
		// ȸ���ϸ鼭 ������������ �̵�
		GetRootComponent()->AddRelativeRotation(m_DeadRotation * deltaTime);
		GetRootComponent()->AddRelativeLocation(m_MoveDirToSpawn * m_DeadSpeed * deltaTime);
	}
	else
	{
		m_bHitted = false;
		GetRootComponent()->SetRelativeRotation(0.f);

		// �̵��ӵ� �ʱ�ȭ
		m_pMovementComponent->ResetVelocityLimit();
		// ���ӵ� �ʱ�ȭ
		m_pMovementComponent->ResetExternalVelocity();

		m_pMovementComponent->SetOriginalVelocityLimit(1.f - (static_cast<float>(m_Money) * 0.05f));
	}
}

void PlayerObject::RevivePlayer()
{
	m_HP = 5;
	m_bStun = false;
	m_bFalling = false;

	// On All Collider
	const std::vector<Collider2D*> thiscolliders = this->GetComponents<Collider2D>();
	for (const auto thisCollider : thiscolliders)
	{
		// �÷��̾��� ���� �ݶ��̴��� ������ �ݶ��̴��� Ư���� �°� �缳�� ���ش�..!
		if (thisCollider->GetName() == L"PartBoxCollider")
			thisCollider->SetCollisionType(COLLISION_TYPE::TRIGGER);
		else
			thisCollider->SetCollisionType(COLLISION_TYPE::COLLISION);
	}

	// �÷��̾� �ѹ��� ���� ���� ȿ�� �ֱ�
	InputManager::GetInstance()->SetControllerVib(m_PlayerNumber, MinVibrationValue, MinVibrationValue);

	// �÷��̾� �����Ʈ �ٽ� ����
	TurnOnPlayerMovement();

	// ���ư� �ؽ��ĸ� ������� ����
	m_MyAngle = 0.f;
	GetRootComponent()->SetRelativeRotation(m_MyAngle);

	// �۾��� �ؽ��ĸ� ������� ����
	m_MyScale = 1.f;
	GetRootComponent()->SetRelativeScale(m_MyScale, m_MyScale);

	// �̵��ӵ� �ʱ�ȭ
	m_pMovementComponent->ResetVelocityLimit();
	// ���ӵ� �ʱ�ȭ
	m_pMovementComponent->ResetExternalVelocity();

	m_pMovementComponent->SetOriginalVelocityLimit(1.f - (static_cast<float>(m_Money) * 0.05f));
}

void PlayerObject::FallOff()
{
	// ��������
	m_bFalling = true;

	// Off All Collider
	const std::vector<Collider2D*> thiscolliders = this->GetComponents<Collider2D>();
	for (const auto thisCollider : thiscolliders)
	{
		thisCollider->SetCollisionType(COLLISION_TYPE::NONE);
	}

	// 2�� ���� ���� ���
	this->SetVibration(2.f, MaxVibrationValue / 4, MaxVibrationValue / 2);

	// �÷��̾� ������ ����
	TurnOffPlayerMovement();

	// �̵��ӵ� �ʱ�ȭ
	m_pMovementComponent->ResetVelocityLimit();
	// ���ӵ� �ʱ�ȭ
	m_pMovementComponent->ResetExternalVelocity();

	m_pMovementComponent->SetOriginalVelocityLimit(1.f - (static_cast<float>(m_Money) * 0.05f));

	// SoundManager::GetInstance()->PlayMusic(eSoundList::StunEffect, eSoundChannel::Effect, 0.3f);
}

void PlayerObject::FallOutEffect(float deltaTime, bool isCliff)
{
	// ũ�� ���
	m_MyScale -= 0.5f * deltaTime;	// 1�ʿ� 0.5��
	if (m_MyScale <= 0.1f)
		m_MyScale = 0.1f;
	this->SetGameObjectScale(m_MyScale, m_MyScale);

	// ȸ��
	m_MyAngle += 900 * deltaTime;	// 1�ʿ� 2.5���� ȸ��
	if (m_MyAngle >= 360.f)
		m_MyAngle -= 360.f;
	this->SetGameObjectRotation(m_MyAngle);

	if (isCliff)
	{
		// �̵�
		if (GetRootComponent()->GetWorldLocation().x < ScreenWidth / 2.f)
			GetRootComponent()->AddRelativeLocation({ -70.f * deltaTime, 0.f });
		else
			GetRootComponent()->AddRelativeLocation({ 70.f * deltaTime, 0.f });
	}
}

void PlayerObject::SetVibration(float vibrationTime, WORD leftVibPower, WORD rightVibPower)
{
	// ���� ���ְ�
	InputManager::GetInstance()->SetControllerVib(m_PlayerNumber, MinVibrationValue, MinVibrationValue);

	// �ٽ� ����
	m_ElapsedVibTime = 0.f;
	m_IsVibration = true;

	m_VibrationTime = vibrationTime;
	m_LeftVibPower = leftVibPower;
	m_RightVibPower = rightVibPower;
}

void PlayerObject::SetVibration2(DWORD playerNumber, float vibrationTime, WORD leftVibPower, WORD rightVibPower)
{
	// ���� ���ְ�
	InputManager::GetInstance()->SetControllerVib(playerNumber, MinVibrationValue, MinVibrationValue);

	// �ٽ� ����
	m_ElapsedVibTime = 0.f;
	m_IsVibration = true;

	m_VibrationTime = vibrationTime;
	m_LeftVibPower = leftVibPower;
	m_RightVibPower = rightVibPower;
}

void PlayerObject::SetShadowAsset()
{
	if (m_name == L"Player1")
	{
		if(m_HP == 0)
			m_pShadowTexture->SetTextureAsset(L"../Resource/black.png", L"");
		else if (m_HP == 2 || m_HP == 1)
			m_pShadowTexture->SetTextureAsset(L"../Resource/red_2.png", L"");
		else if (m_HP == 4 || m_HP == 3)
			m_pShadowTexture->SetTextureAsset(L"../Resource/red_1.png", L"");
		else if (m_HP == 5)
			m_pShadowTexture->SetTextureAsset(L"../Resource/red.png", L"");
	}
	else if (m_name == L"Player2")
	{
		if (m_HP == 0)
			m_pShadowTexture->SetTextureAsset(L"../Resource/black.png", L"");
		else if (m_HP == 2 || m_HP == 1)
			m_pShadowTexture->SetTextureAsset(L"../Resource/blue_2.png", L"");
		else if (m_HP == 4 || m_HP == 3)
			m_pShadowTexture->SetTextureAsset(L"../Resource/blue_1.png", L"");
		else if (m_HP == 5)
			m_pShadowTexture->SetTextureAsset(L"../Resource/blue.png", L"");
	}
	else if (m_name == L"Player3")
	{
		if (m_HP == 0)
			m_pShadowTexture->SetTextureAsset(L"../Resource/black.png", L"");
		else if (m_HP == 2 || m_HP == 1)
			m_pShadowTexture->SetTextureAsset(L"../Resource/yellow_2.png", L"");
		else if (m_HP == 4 || m_HP == 3)
			m_pShadowTexture->SetTextureAsset(L"../Resource/yellow_1.png", L"");
		else if (m_HP == 5)
			m_pShadowTexture->SetTextureAsset(L"../Resource/yellow.png", L"");
	}
	else if (m_name == L"Player4")
	{
		if (m_HP == 0)
			m_pShadowTexture->SetTextureAsset(L"../Resource/black.png", L"");
		else if (m_HP == 2 || m_HP == 1)
			m_pShadowTexture->SetTextureAsset(L"../Resource/green_2.png", L"");
		else if (m_HP == 4 || m_HP == 3)
			m_pShadowTexture->SetTextureAsset(L"../Resource/green_1.png", L"");
		else if (m_HP == 5)
			m_pShadowTexture->SetTextureAsset(L"../Resource/green.png", L"");
	}
}
