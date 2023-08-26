#pragma once

#include "RenderComponent.h"

class AnimationAsset;

/// <summary>
/// 애니메이션 에셋 정보를 통해 실제 오브젝트에 붙을 애니메이션 컴포넌트.
/// FSM에서 AnimationComponent를 조작하기 위해 HandleEvent를 구현한다.
/// 에셋 정보를 Save 해주고, Json 파일이 생성되면 Load 해준다.
/// </summary>
class AnimationComponent
	: public RenderComponent
{
protected:
	AnimationAsset* m_pAnimationAsset = nullptr;									// 애니메이션을 위한 기본 데이터 자산 (내부에 D2D1Bitmap)

	std::wstring m_AnimationName;													// 현재 애니메이션 이름
	size_t m_FrameIndex = 0;														// 현재 프레임 인덱스
	float m_ProgressTime = 0.f;														// 애니메이션 진행시간
	D2D1_RECT_F m_SrcRect = { 0.f, 0.f, 0.f, 0.f };			// 시간에 따른 D2D1Bitmap의 Source 영역
	D2D1_RECT_F m_DstRect = { 0.f, 0.f, 0.f, 0.f };			// 시간에 따른 D2D1Bitmap의 Source 영역
	bool m_bMirror = false;
	float m_Speed = 1.f;

	bool m_bLoop = true;
	bool m_bActive = false;

public:
	AnimationComponent();
	virtual ~AnimationComponent();

public:
	void SetSpeed(const float val) { m_Speed = val; }
	void SetAnimationName(std::wstring name) { m_AnimationName = name; }

	void SetLoop(bool loop) { m_bLoop = loop; }
	void SetActive(bool active) { m_bActive = active; }

public:
	void Update(const float deltaTime) final;
	void Render(ID2D1RenderTarget* pRenderTarget) final;

public:
	void SetAnimationBitmapFilePath(const WCHAR* szFilePath);
	void SetAnimationAsset(const WCHAR* szFilePath, const WCHAR* animationName, std::vector<framework::FRAME_INFO> frameInfo);
	void LoadAnimationAsset(const WCHAR* szFilePath, const WCHAR* assetName);

	void ChangeAnimation(const std::wstring name, const bool flip);
	void KeepAnimation(const std::wstring name, const bool flip);
};