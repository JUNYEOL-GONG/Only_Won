#include "framework.h"
#include "WaveItem.h"

#include "InGameWorld.h"
#include "ItemBoxObject.h"
#include "ItemObject.h"
#include "PlayerObject.h"

#include "../Engine/AnimationComponent.h"
#include "../Engine/EventManager.h"
#include "../Engine/World.h"
#include "../Engine/SoundManager.h"

bool WaveItem::Initialize()
{
	m_pEffectAnimation = CreateComponent<AnimationComponent>(L"EffectAnimation");
	std::vector<framework::FRAME_INFO> effectFrame;
	effectFrame.emplace_back(framework::FRAME_INFO{ 0, 0, 1920, 1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1920, 0, 1920, 1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 3840, 0, 1920,1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 5760, 0, 1920,1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 7680, 0, 1920,1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 9600, 0, 1920, 1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 11520, 0, 1920, 1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 0, 1080, 1920, 1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1920, 1080, 1920, 1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 3840, 1080, 1920, 1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 5760, 1080, 1920, 1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 7680, 1080, 1920, 1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 9600, 1080, 1920, 1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 11520, 1080, 1920, 1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 0, 2160, 1920, 1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 1920, 2160, 1920, 1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 3840, 2160, 1920, 1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 5760, 2160, 1920, 1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 7680, 2160, 1920, 1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 9600, 2160, 1920, 1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 11520, 2160, 1920, 1080, 0.1f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 0, 0, 0, 0, 0.1f });
	m_pEffectAnimation->SetAnimationAsset(L"../Resource/Effect_Wave.png", L"WaveEffect", effectFrame);
	m_pEffectAnimation->SetLoop(false);
	m_pEffectAnimation->SetAnimationName(L"WaveEffect");
	m_pEffectAnimation->SetActive(true);
	m_pEffectAnimation->SetRelativeLocation(ScreenWidth / 2.f, ScreenHeight / 2.f);
	SetRootComponent(m_pEffectAnimation);

	// TODO : ��� �÷��̾��� ����
	InputManager::GetInstance()->SetAllControllerVib(MaxVibrationValue, MaxVibrationValue);

	// ������
	//std::vector<GameObject*>& worldAllPlayer = m_pOwnerWorld->GetGameObjectGroup(GROUP_TYPE::PLAYER);
	//// �÷��̾� �� ������
	//for (auto& respawnPlayer : worldAllPlayer)
	//{
	//	respawnPlayer->SetGameObjectLocation(300.f, 300.f);
	//}

	std::vector<GameObject*>& worldAllItemBox = m_pOwnerWorld->GetGameObjectGroup(GROUP_TYPE::ITEMBOX);

	// �����۹ڽ� �� �����
	for(auto& deleteAllItemBox : worldAllItemBox)
	{
		EventManager::GetInstance()->SendEvent(eEventType::DeleteGameObject, GROUP_TYPE::ITEMBOX, deleteAllItemBox);

		if(dynamic_cast<ItemBoxObject*>(deleteAllItemBox)->GetItemBoxType() == eItemBoxType::THROW)
		{
			EventManager::GetInstance()->SendEvent(eEventType::DeleteThrow);
		}
		else if (dynamic_cast<ItemBoxObject*>(deleteAllItemBox)->GetItemBoxType() == eItemBoxType::REINFORCED)
		{
			EventManager::GetInstance()->SendEvent(eEventType::DeleteReinforced);
		}
		else if (dynamic_cast<ItemBoxObject*>(deleteAllItemBox)->GetItemBoxType() == eItemBoxType::INSTALLATION)
		{
			EventManager::GetInstance()->SendEvent(eEventType::DeleteInstallation);
		}
	}

	std::vector<GameObject*>& worldAllItem = m_pOwnerWorld->GetGameObjectGroup(GROUP_TYPE::ITEM);

	// ������ �� �����
	for (auto deleteItem : worldAllItem)
	{
		ItemObject* castedItem = dynamic_cast<ItemObject*>(deleteItem);
		if(castedItem->GetItemBoxType() == eItemBoxType::INSTALLATION)
		{
			EventManager::GetInstance()->SendEvent(eEventType::DeleteGameObject, GROUP_TYPE::ITEM, castedItem);
		}
	}

	// �������� �� ���ڸ��� ���
	for(auto& player : dynamic_cast<InGameWorld*>(m_pOwnerWorld)->GetPlayerList())
	{
		player->GetDamaged(5, player->GetMoney());
		player->MoveToSpawnImmediately();
	}

	SoundManager::GetInstance()->PlayMusic(eSoundList::WaveEffect, eSoundChannel::MapState, 1.f);

	dynamic_cast<PlayerObject*>(m_pOwnerPlayer)->SetReinforcedNothing();

	bool res = __super::Initialize();
	assert(res);

	return true;
}

void WaveItem::Update(const float deltaTime)
{
	__super::Update(deltaTime);
}