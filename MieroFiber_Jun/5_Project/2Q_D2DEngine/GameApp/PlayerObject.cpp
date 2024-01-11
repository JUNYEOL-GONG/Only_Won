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

	// 플레이어 번호 설정 / 시작 위치 설정
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

	// 가진 아이템 초기화
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

	// 처음은 너구리가 앞에 보고 있으니까 true
	m_bIsLookFront = true;

	// PlayerMovement
	m_pMovementComponent = CreateComponent<PlayerMovement>(L"PlayerMovement");

	// FSMComponent - AttatchFSM은 아래 인스턴스화한 Player에서 한다
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

	// 아이템 사용횟수 설정
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
	/// 플레이어 시간만큼 진동 로직
	if (!m_IsTrainVib)
	{
		if (m_IsVibration)
		{
			m_ElapsedVibTime += deltaTime;

			InputManager::GetInstance()->SetControllerVib(m_PlayerNumber, m_LeftVibPower, m_RightVibPower);

			// 진동 시간 끝나면
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

	/// 스턴 상태 전환 확인
	if (m_HP <= 0 && !m_bStun)
	{
		TurnToStun();
	}

	/// LookDir에 따라서 돈보따리 왔다리 갔다리
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

	/// 보유한 돈 스코어로 전환
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

	/// 플레이어 회복 타이머
	if(!m_bStun && !PlayerGetDamagedTimer(deltaTime))
	{
		if (m_HP < 5)
		{
			m_HP++;
		}
	}

	/// 아이템 쿨 타이머
	ItemCoolTimer(deltaTime);

	/// 이펙트 재생
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

	/// 현재 소지한 돈 + 모아둔 돈이 목표금액 이상인 경우 이펙트 출력
	if ((m_Score + m_Money) >= dynamic_cast<InGameWorld*>(m_pOwnerWorld)->GetGoalScore())
	{
		m_pWinnableTexture->SetVisible(true);
		m_pWinnableTexture->AddRelativeRotation(deltaTime * 50.f);

		// 실행...!
		if (!m_IsWinnableVib)
		{
			const std::vector<PlayerObject*> playerList = dynamic_cast<InGameWorld*>(this->GetOwnerWorld())->GetPlayerList();

			for (const auto& player : playerList)
			{
				// 나를 제외한 모든 플레이이어
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

	/// 스턴 상태가 아닐 때만 실행
	if (!m_bStun)
	{
		// 떨어지는 중이 아닐때
		if (!m_bFalling)
		{
			// 맵 밖으로 나간 경우
			if (IsOutOfRange())
			{
				// 떨어짐
				FallOff();
			}
		}
		// 떨어지기 시작
		else
		{
			// 절벽에서 떨어진 경우
			if (IsOutOfRange())
			{
				FallOutEffect(deltaTime, true);
			}
			// 함정으로 떨어진 경우
			else
			{
				// 떨어지는 연출
				FallOutEffect(deltaTime, false);
			}

			static float fallingTime = 0.f;
			fallingTime += deltaTime;

			// 플레이어 살리기
			if (fallingTime >= 2.f)
			{
				fallingTime = 0.f;
				RevivePlayer();
				MoveToSpawnImmediately();
			}
		}

		// 아이템 사용 가능한 경우
		if (m_IsItemUsable)
		{
			// 아이템 사용
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
	/// 스턴 상태일 경우 실행
	else
	{
		// 기차와 충돌한 경우
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

		// 스턴 3.5초 타이머
		m_StunTimer += deltaTime;

		if(m_StunTimer >= 3.5f)
		{
			m_StunTimer = 0.f;
			RevivePlayer();
			m_stunPlayTime = false;
		}
	}

	/// HP STUN 상태에 반영하도록 SendEvent
	SendHPToFSM();
	/// hp에 따라서 그림자 바꿔주기
	SetShadowAsset();
	/// 보따리 사이즈 정하기
	SetBundleSize();

	/// 돈보따리가 투명화일 때 안 보이게 하기
	if (m_bTransparencing)
	{
		m_pBundleTexture->SetShowTexture(false);
		m_pShadowTexture->SetShowTexture(false);
	}
	else
	{
		// 돈이 하나도 없을 때는 보따리 안 보이게 하기
		if(m_Money == 0)
			m_pBundleTexture->SetShowTexture(false);
		else
			m_pBundleTexture->SetShowTexture(true);
		m_pShadowTexture->SetShowTexture(true);
	}

	// 플레이어 컴포넌트 업데이트
	GameObject::Update(deltaTime);
}

void PlayerObject::OnCollisionEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{
	// 기차의 헤드 콜라이더라면
	if (otherCollision->GetName() == L"BoxCollider2D_Head")
	{
		// Exit 처리
		thisCollision->TurnOff_IsCollision(otherCollision);

		iter->second = false;

		otherCollision->OnCollisionExit(thisCollision);
		thisCollision->OnCollisionExit(otherCollision);

		m_bHitted = true;

		// Money 드랍
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
		// 플레이어 넘버에 따른 진동 효과 주기
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
		// 플레이어 넘버에 따른 진동 효과 주기
		InputManager::GetInstance()->SetControllerVib(m_PlayerNumber, MinVibrationValue, MinVibrationValue);
		m_IsTrainVib = false;
	}
}

void PlayerObject::OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{
	ItemBoxObject* itemBoxObj = dynamic_cast<ItemBoxObject*>(otherCollision->GetOwner());
	MoneyObject* moneyObj = dynamic_cast<MoneyObject*>(otherCollision->GetOwner());

	// ItemBox와 충돌 (플레이어의 몸 콜라이더에만 해당)
	if (itemBoxObj && thisCollision == GetBoxColliderFull())
	{
		// 아이템 박스 타입
		const eItemBoxType itemBoxType = itemBoxObj->GetItemBoxType();

		// 해당 플레이어가 아이템 박스 타입에 대한 아이템을 이미 가지고 있지 않아 아이템을 랜덤으로 얻을 수 있는 경우
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

			// 아이템 박스 삭제
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

			// Exit 처리 해주기
			thisCollision->TurnOff_IsTrigger(otherCollision);

			iter->second = false;

			otherCollision->OnTriggerExit(thisCollision);
			thisCollision->OnTriggerExit(otherCollision);
		}
	}
	// MoneyObject와 충돌
	else if (moneyObj
		&& (moneyObj->GetMoneyType() == eMoneyType::DROPED_COLLISION
			|| moneyObj->GetMoneyType() == eMoneyType::SPAWNED)
		&& m_Money < 10 && thisCollision == GetBoxColliderFull())
	{
		EventManager::GetInstance()->SendEvent(eEventType::DeleteMoney);
		EventManager::GetInstance()->SendEvent(eEventType::DeleteGameObject, GROUP_TYPE::MONEY, otherCollision->GetOwner());

		// Exit 처리 해주기
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

		// 진동 재생
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
		// 투명화 시작
		if (event->eventID == eEventType::Player1TransparentStart)
		{
			m_pAnimComponent->SetVisible(false);
			return;
		}
		// 투명화 끝
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
		// 투명화 시작
		if (event->eventID == eEventType::Player2TransparentStart)
		{
			m_pAnimComponent->SetVisible(false);
			return;
		}
		// 투명화 끝
		if (event->eventID == eEventType::Player2TransparentEnd)
		{
			m_pAnimComponent->SetVisible(true);
			return;
		}
		// 투명화 시작
		if (event->eventID == eEventType::Player2TransparentStart)
		{
			m_pAnimComponent->SetVisible(false);
			return;
		}
		// 투명화 끝
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
		// 투명화 시작
		if (event->eventID == eEventType::Player3TransparentStart)
		{
			m_pAnimComponent->SetVisible(false);
			return;
		}
		// 투명화 끝
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
		// 투명화 시작
		if (event->eventID == eEventType::Player4TransparentStart)
		{
			m_pAnimComponent->SetVisible(false);
			return;
		}
		// 투명화 끝
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

// OnTriggerEnter에서 상대 Object가 ItemBox인지 확인하고 호출
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

			// 획득한 아이템 타입
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

			// m_PlayerUsageCnt : 플레이어가 그 아이템을 들고있으면 얼마나 더 쓸 수 있는지 / m_ItemUsageCnt : 아이템 자체 정보
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

	// 사용한 아이템이 투사체 아이템의 경우 설정 필요
	if (itemType == eItemType::PunchItem || itemType == eItemType::SnowItem || itemType == eItemType::StungunItem || itemType == eItemType::BottleItem)
	{
		// 투사체 아이템을 모두 사용한 경우 Clear 텍스쳐로 변경
		if(m_PlayerUsageCnt[static_cast<UINT>(eItemBoxType::THROW)] <= 1)
		{
			// std::cout << m_PlayerUsageCnt << std::endl;
			DirectionComponent* directionComponent = this->GetComponent<DirectionComponent>();
			directionComponent->SetTextureAsset(L"../Resource/clear.png", L"Clear");
		}

		// ItemObject를 ProjectileObject로 캐스팅
		ProjectileObject* projectile = dynamic_cast<ProjectileObject*>(m_pCreatedItem);

		// 아이템의 소유자 설정
		projectile->SetOwnerPlayer(this);

		// 속도 설정 (투사체의 기본 속도를 이용해도 된다)
		projectile->SetSpeed(1000.f);

		// 바라보는 방향에 따른 회전 설정
		const framework::Vector2D tempLookDir = GetComponent<PlayerMovement>()->GetLookDir();
		float theta = atan(tempLookDir.y / tempLookDir.x); // 아크탄젠트 함수를 사용해 theta를 구함
		theta = theta * 180.f / 3.14159265f; // 라디안을 도로 변환
		projectile->SetRotation(theta);

		// 방향 설정
		projectile->SetDirection(tempLookDir);

		// 투사체 아이템의 생성 위치 설정
		const framework::Vector2D pos = GetRootComponent()->GetRelativeLocation();
		constexpr float distance = 75.f;
		const framework::Vector2D distVec = tempLookDir * distance;
		projectile->SetPosition(pos + distVec);

		// 투사체 아이템에 컴포넌트 투착 초기화
		projectile->Initialize();

		// 투사체 아이템의 이미지 반전
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

	// 진동 재생
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
	// 아이템 쿨타임 타이머
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
	// 2초 지났는지 안 지났는지
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
	// 날아가는 속도 벡터 계산
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
	// 화면 밖으로 넘어감

	// 절벽
	if (this->GetRootComponent()->GetWorldLocation().x > ScreenWidth - 70.f || this->GetRootComponent()->GetWorldLocation().x < 0 + 70.f)
	{
		return true;
	}

	// 위아래
	if (this->GetRootComponent()->GetWorldLocation().y >= ScreenHeight - 50.f)
	{
		this->SetGameObjectLocation(this->GetRootComponent()->GetWorldLocation().x, ScreenHeight - 50.f);
	}
	if (this->GetRootComponent()->GetWorldLocation().y <= 0)
	{
		this->SetGameObjectLocation(this->GetRootComponent()->GetWorldLocation().x, 0.f);
	}

	// 0, 0에 가까워지면 보내버리자
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

	// 2초 동안 진동 재생
	this->SetVibration(2.f, MaxVibrationValue / 4, MaxVibrationValue / 2);

	// 플레이어 움직임 끄기
	TurnOffPlayerMovement();

	// 플레이어 사망시 날아가는 속도 계산
	CalculateDeadVelocity();
}

void PlayerObject::MoveToSpawnImmediately()
{
	this->SetGameObjectLocation(m_SpawnPosition.x, m_SpawnPosition.y);

	// 이동속도 초기화
	m_pMovementComponent->ResetVelocityLimit();
	// 가속도 초기화
	m_pMovementComponent->ResetExternalVelocity();

	m_pMovementComponent->SetOriginalVelocityLimit(1.f - (static_cast<float>(m_Money) * 0.05f));
}

void PlayerObject::MoveToSpawnWithRotation(float deltaTime)
{
	if(framework::Vector2D::Distance(GetRootComponent()->GetWorldLocation(), m_SpawnPosition) > 10.f)
	{
		// 회전하면서 스폰지역으로 이동
		GetRootComponent()->AddRelativeRotation(m_DeadRotation * deltaTime);
		GetRootComponent()->AddRelativeLocation(m_MoveDirToSpawn * m_DeadSpeed * deltaTime);
	}
	else
	{
		m_bHitted = false;
		GetRootComponent()->SetRelativeRotation(0.f);

		// 이동속도 초기화
		m_pMovementComponent->ResetVelocityLimit();
		// 가속도 초기화
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
		// 플레이어의 여러 콜라이더를 각각의 콜라이더의 특성에 맞게 재설정 해준다..!
		if (thisCollider->GetName() == L"PartBoxCollider")
			thisCollider->SetCollisionType(COLLISION_TYPE::TRIGGER);
		else
			thisCollider->SetCollisionType(COLLISION_TYPE::COLLISION);
	}

	// 플레이어 넘버에 따른 진동 효과 주기
	InputManager::GetInstance()->SetControllerVib(m_PlayerNumber, MinVibrationValue, MinVibrationValue);

	// 플레이어 무브먼트 다시 켜줌
	TurnOnPlayerMovement();

	// 돌아간 텍스쳐를 원래대로 돌림
	m_MyAngle = 0.f;
	GetRootComponent()->SetRelativeRotation(m_MyAngle);

	// 작아진 텍스쳐를 원래대로 돌림
	m_MyScale = 1.f;
	GetRootComponent()->SetRelativeScale(m_MyScale, m_MyScale);

	// 이동속도 초기화
	m_pMovementComponent->ResetVelocityLimit();
	// 가속도 초기화
	m_pMovementComponent->ResetExternalVelocity();

	m_pMovementComponent->SetOriginalVelocityLimit(1.f - (static_cast<float>(m_Money) * 0.05f));
}

void PlayerObject::FallOff()
{
	// 떨어진다
	m_bFalling = true;

	// Off All Collider
	const std::vector<Collider2D*> thiscolliders = this->GetComponents<Collider2D>();
	for (const auto thisCollider : thiscolliders)
	{
		thisCollider->SetCollisionType(COLLISION_TYPE::NONE);
	}

	// 2초 동안 진동 재생
	this->SetVibration(2.f, MaxVibrationValue / 4, MaxVibrationValue / 2);

	// 플레이어 움직임 끄기
	TurnOffPlayerMovement();

	// 이동속도 초기화
	m_pMovementComponent->ResetVelocityLimit();
	// 가속도 초기화
	m_pMovementComponent->ResetExternalVelocity();

	m_pMovementComponent->SetOriginalVelocityLimit(1.f - (static_cast<float>(m_Money) * 0.05f));

	// SoundManager::GetInstance()->PlayMusic(eSoundList::StunEffect, eSoundChannel::Effect, 0.3f);
}

void PlayerObject::FallOutEffect(float deltaTime, bool isCliff)
{
	// 크기 축소
	m_MyScale -= 0.5f * deltaTime;	// 1초에 0.5배
	if (m_MyScale <= 0.1f)
		m_MyScale = 0.1f;
	this->SetGameObjectScale(m_MyScale, m_MyScale);

	// 회전
	m_MyAngle += 900 * deltaTime;	// 1초에 2.5바퀴 회전
	if (m_MyAngle >= 360.f)
		m_MyAngle -= 360.f;
	this->SetGameObjectRotation(m_MyAngle);

	if (isCliff)
	{
		// 이동
		if (GetRootComponent()->GetWorldLocation().x < ScreenWidth / 2.f)
			GetRootComponent()->AddRelativeLocation({ -70.f * deltaTime, 0.f });
		else
			GetRootComponent()->AddRelativeLocation({ 70.f * deltaTime, 0.f });
	}
}

void PlayerObject::SetVibration(float vibrationTime, WORD leftVibPower, WORD rightVibPower)
{
	// 진동 꺼주고
	InputManager::GetInstance()->SetControllerVib(m_PlayerNumber, MinVibrationValue, MinVibrationValue);

	// 다시 시작
	m_ElapsedVibTime = 0.f;
	m_IsVibration = true;

	m_VibrationTime = vibrationTime;
	m_LeftVibPower = leftVibPower;
	m_RightVibPower = rightVibPower;
}

void PlayerObject::SetVibration2(DWORD playerNumber, float vibrationTime, WORD leftVibPower, WORD rightVibPower)
{
	// 진동 꺼주고
	InputManager::GetInstance()->SetControllerVib(playerNumber, MinVibrationValue, MinVibrationValue);

	// 다시 시작
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
