#include "framework.h"
#include "TrafficObject.h"

#include "../Engine/TextureComponent.h"
#include "../Engine/BoxCollider2D.h"
#include "../Engine/AnimationComponent.h"
#include "../Engine/SoundManager.h"

TrafficObject::TrafficObject()
{
}

TrafficObject::~TrafficObject()
{
}

bool TrafficObject::Initialize()
{
	m_playTime = false;

	/// TextureComponent
	m_pTextureComponent = CreateComponent<TextureComponent>(L"TextureComponent");
	m_pTextureComponent->SetTextureAsset(L"../Resource/clear.png", L"Clear");
	SetRootComponent(m_pTextureComponent);

	/// BoxCollider2D
	m_pBoxCollider2D = CreateComponent<BoxCollider2D>(L"BoxCollider2D");
	m_pBoxCollider2D->AttachToComponent(m_pTextureComponent);

	/// AnimationComponent
	m_pEffectAnimation = CreateComponent<AnimationComponent>(L"EffectAnimation");
	std::vector<framework::FRAME_INFO> effectFrame;
	effectFrame.emplace_back(framework::FRAME_INFO{ 0, 0, 60, 210, 0.3f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 60, 0, 60, 210, 0.3f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 120, 0, 60, 210, 0.3f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 180, 0, 60, 210, 0.3f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 240, 0, 60, 210, 0.3f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 300, 0, 60, 210, 0.3f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 360, 0, 60, 210, 0.3f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 420, 0, 60, 210, 0.3f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 480, 0, 60, 210, 0.3f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 540, 0, 60, 210, 0.3f });
	effectFrame.emplace_back(framework::FRAME_INFO{ 0, 0, 0, 0, 0.1f });
	m_pEffectAnimation->SetAnimationAsset(L"../Resource/Effect_TrafficLight.png", L"TrafficEffect", effectFrame);
	m_pEffectAnimation->SetAnimationName(L"TrafficEffect");
	m_pEffectAnimation->SetLoop(false);
	m_pEffectAnimation->SetActive(false);
	m_pEffectAnimation->AttachToComponent(m_pTextureComponent);

	const bool res = GameObject::Initialize();
	assert(res);

	return true;
}

void TrafficObject::Update(float deltaTime)
{
	// 컴포넌트 업데이트
	GameObject::Update(deltaTime);
}

void TrafficObject::StartAnimation()
{
	if(!m_playTime)
	{
		SoundManager::GetInstance()->StopMusic(eSoundChannel::Train);
		SoundManager::GetInstance()->PlayMusic(eSoundList::SignalEffect, eSoundChannel::TrainSignal, 0.9f);
		m_playTime = true;
	}
	m_pEffectAnimation->SetActive(true);
}

void TrafficObject::OnCollisionEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{

}

void TrafficObject::OnCollisionStay(Collider2D* otherCollision)
{

}

void TrafficObject::OnCollisionExit(Collider2D* otherCollision)
{

}

void TrafficObject::OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{

}

void TrafficObject::OnTriggerStay(Collider2D* otherCollision)
{

}

void TrafficObject::OnTriggerExit(Collider2D* otherCollision)
{

}

void TrafficObject::SetTexture(const WCHAR* szFilePath, const std::wstring textureName) const
{
	m_pTextureComponent->SetTextureAsset(szFilePath, textureName);
}

void TrafficObject::SetObstaclePos(float x, float y) const
{
	m_pTextureComponent->SetRelativeLocation(x, y);
}

void TrafficObject::SetObstacleRotation(float angle) const
{
	m_pTextureComponent->SetRelativeRotation(angle);
}

void TrafficObject::SetColliderPos(float x, float y) const
{
	m_pBoxCollider2D->SetRelativeLocation(x, y);
}

void TrafficObject::SetColliderExtend(float x, float y) const
{
	m_pBoxCollider2D->SetExtend(x, y);
}