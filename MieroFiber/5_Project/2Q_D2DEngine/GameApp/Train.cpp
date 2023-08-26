#include "framework.h"
#include "Train.h"

#include "TrainSpawner.h"
#include "TrainPattern.h"

#include "PlayerObject.h"
#include "../Engine/PlayerMovement.h"
#include "../Engine/TextureComponent.h"
#include "../Engine/BoxCollider2D.h"

#include "../Engine/CommonApp.h"
#include "../Engine/EventManager.h"
#include "../Engine/SoundManager.h"

Train::Train()
{
}

Train::~Train()
{
}

bool Train::Initialize()
{
	/// TextrueComponent
	m_pTextureComponent = CreateComponent<TextureComponent>(L"TextureComponent");
	if (m_TrainType == TRAIN_TYPE::ABC)
		m_pTextureComponent->SetTextureAsset(L"../Resource/Train_1.png", L"TrainTexture");
	else if (m_TrainType == TRAIN_TYPE::ABB)
		m_pTextureComponent->SetTextureAsset(L"../Resource/Train_2.png", L"TrainTexture");
	else if (m_TrainType == TRAIN_TYPE::ABD)
		m_pTextureComponent->SetTextureAsset(L"../Resource/Train_3.png", L"TrainTexture");
	else if (m_TrainType == TRAIN_TYPE::ABCB)
		m_pTextureComponent->SetTextureAsset(L"../Resource/Train_4.png", L"TrainTexture");
	else if (m_TrainType == TRAIN_TYPE::ABBBB)
		m_pTextureComponent->SetTextureAsset(L"../Resource/Train_5.png", L"TrainTexture");
	else
		assert(false);
	m_pTextureComponent->SetRelativeLocation(m_SpawnPosition.x, m_SpawnPosition.y);
	SetRootComponent(m_pTextureComponent);

	// 기다란 기차
	if (m_TrainType == TRAIN_TYPE::ABBBB)
	{
		/// BoxCollider2D
		m_pBoxCollider2D = CreateComponent<BoxCollider2D>(L"Train_Body");
		m_pBoxCollider2D->SetRelativeLocation(0.f, static_cast<float>(m_Direction) * -10.f);
		m_pBoxCollider2D->SetExtend(80.f, 1240.f);
		m_pBoxCollider2D->AttachToComponent(m_pTextureComponent);

		/// BoxCollider2D
		m_pHeadBoxCollider = CreateComponent<BoxCollider2D>(L"BoxCollider2D_Head");
		m_pHeadBoxCollider->SetRelativeLocation(0.f, static_cast<float>(m_Direction) * 1240.f);
		m_pHeadBoxCollider->SetExtend(80.f, 10.f);
		m_pHeadBoxCollider->AttachToComponent(m_pTextureComponent);
	}
	// 짧은 기차
	else
	{
		/// BoxCollider2D
		m_pBoxCollider2D = CreateComponent<BoxCollider2D>(L"Train_Body");
		m_pBoxCollider2D->SetRelativeLocation(0.f, static_cast<float>(m_Direction) * -10.f);
		m_pBoxCollider2D->SetExtend(80.f, 740.f);
		m_pBoxCollider2D->AttachToComponent(m_pTextureComponent);

		/// BoxCollider2D
		m_pHeadBoxCollider = CreateComponent<BoxCollider2D>(L"BoxCollider2D_Head");
		m_pHeadBoxCollider->SetRelativeLocation(0.f, static_cast<float>(m_Direction) * 740.f);
		m_pHeadBoxCollider->SetExtend(80.f, 10.f);
		m_pHeadBoxCollider->AttachToComponent(m_pTextureComponent);
	}

	// 속도 벡터 생성
	m_Velocity = { 0.f, m_Speed * static_cast<float>(m_Direction) };

	SoundManager::GetInstance()->PlayMusic(eSoundList::TrainEffect, eSoundChannel::Train, 0.9f);

	const bool res = GameObject::Initialize();
	assert(res);

	return true;
}

void Train::Update(const float deltaTime)
{
	/// 만약 화면 벗어나면 삭제
	const float extendY = dynamic_cast<BoxCollider2D*>(this->GetComponent<Collider2D>())->GetExtend().y;
	if(m_SpawnPosition.y < 0.f)
	{
		if (this->GetRootComponent()->GetWorldLocation().y - extendY > ScreenHeight)
		{
			m_pSpawner->SetIsRunning(true);
			EventManager::GetInstance()->SendEvent(eEventType::DeleteGameObject, GROUP_TYPE::TRAIN, this);
			return;
		}
	}
	else if (m_SpawnPosition.y > ScreenHeight)
	{
		if (this->GetRootComponent()->GetWorldLocation().y + extendY < 0)
		{
			m_pSpawner->SetIsRunning(true);
			EventManager::GetInstance()->SendEvent(eEventType::DeleteGameObject, GROUP_TYPE::TRAIN, this);
			return;
		}
	}

	// 기차의 이동
	GetRootComponent()->AddRelativeLocation(m_Velocity * deltaTime);

	// 컴포넌트 업데이트
	GameObject::Update(deltaTime);
}

void Train::OnCollisionEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{

}

void Train::OnCollisionStay(Collider2D* otherCollision)
{

}

void Train::OnCollisionExit(Collider2D* otherCollision)
{

}

void Train::OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter)
{

}

void Train::OnTriggerStay(Collider2D* otherCollision)
{

}

void Train::OnTriggerExit(Collider2D* otherCollision)
{

}