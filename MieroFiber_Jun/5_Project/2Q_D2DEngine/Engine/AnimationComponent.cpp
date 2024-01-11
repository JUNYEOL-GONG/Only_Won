#include "pch.h"
#include "AnimationComponent.h"

#include "AnimationAsset.h"
#include "GameObject.h"
#include "EventManager.h"

AnimationComponent::AnimationComponent()
{
	m_pAnimationAsset = new AnimationAsset;
}

AnimationComponent::~AnimationComponent()
{
	delete m_pAnimationAsset;
}

void AnimationComponent::Update(const float deltaTime)
{
	// 한 번만 재생하는 타입인데 활성화가 안되어 있으면 return
	if(!m_bLoop && !m_bActive)
	{
		return;
	}

	m_ProgressTime += deltaTime;
	std::vector<framework::FRAME_INFO> curAnimation = m_pAnimationAsset->m_Animations.find(m_AnimationName)->second;
	float renderTime = curAnimation[m_FrameIndex].RenderTime;

	if (m_ProgressTime >= renderTime)
	{
		m_ProgressTime -= renderTime;
		m_FrameIndex = (m_FrameIndex + 1) % (curAnimation.size());
		m_SrcRect = curAnimation[m_FrameIndex].Source;
		m_DstRect = { -(m_SrcRect.right - m_SrcRect.left) / 2.f, -(m_SrcRect.bottom - m_SrcRect.top) / 2.f,
			(m_SrcRect.right - m_SrcRect.left) / 2.f, (m_SrcRect.bottom - m_SrcRect.top) / 2.f };
	}

	// 한 번만 재생하는 타입인데 이번 프레임이 마지막 프레임이었으면 비활성화
	if(!m_bLoop && (m_FrameIndex == curAnimation.size() - 1))
	{
		m_bActive = false;
		m_FrameIndex = 0;
	}

	__super::Update(deltaTime);
}

void AnimationComponent::Render(ID2D1RenderTarget* pRenderTarget)
{
	if (!m_bVisible)
		return;

	if (m_bMirror)
	{
		m_WorldTransform = D2D1::Matrix3x2F::Scale(D2D1::SizeF(-1.f, 1.f))
			* m_WorldTransform;
	}
	m_FinalTransform = m_WorldTransform * GetCameraTransform();
	pRenderTarget->SetTransform(m_FinalTransform);

	pRenderTarget->DrawBitmap(
		m_pAnimationAsset->m_pBitmap,
		m_DstRect,
		1.f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		m_SrcRect);

	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void AnimationComponent::SetAnimationBitmapFilePath(const WCHAR* szFilePath)
{
	m_pAnimationAsset->SetBitmapFilePath(szFilePath);
	m_pAnimationAsset->Build();
	//m_pAnimationAsset->m_Animations.insert(std::pair<std::wstring, std::vector<framework::FRAME_INFO>>(animationName, frameInfo));
}

void AnimationComponent::SetAnimationAsset(const WCHAR* szFilePath, const WCHAR* animationName, std::vector<framework::FRAME_INFO> frameInfo)
{
	m_pAnimationAsset->SetBitmapFilePath(szFilePath);
	m_pAnimationAsset->Build();
	m_pAnimationAsset->m_Animations.insert(std::pair<std::wstring, std::vector<framework::FRAME_INFO>>(animationName, frameInfo));
	m_pAnimationAsset->Save(L"../Resource/Animation.WorldAsset");
}


void AnimationComponent::LoadAnimationAsset(const WCHAR* szFilePath, const WCHAR* assetName)
{
	m_pAnimationAsset->SetBitmapFilePath(szFilePath);
	m_pAnimationAsset->Build();
	m_pAnimationAsset->Load(assetName);
}

void AnimationComponent::ChangeAnimation(const std::wstring name, const bool flip)
{
	assert(m_pAnimationAsset != nullptr);
	assert(m_pAnimationAsset->m_Animations.find(name) != m_pAnimationAsset->m_Animations.end());

	m_AnimationName = name;
	m_ProgressTime = 0.f;
	m_FrameIndex = 0;
	m_bMirror = flip;
}

void AnimationComponent::KeepAnimation(const std::wstring name, const bool flip)
{
	assert(m_pAnimationAsset != nullptr);

	m_AnimationName = name;
	m_bMirror = flip;
}
