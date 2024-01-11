#pragma once

#include "RenderComponent.h"

class AnimationAsset;

/// <summary>
/// �ִϸ��̼� ���� ������ ���� ���� ������Ʈ�� ���� �ִϸ��̼� ������Ʈ.
/// FSM���� AnimationComponent�� �����ϱ� ���� HandleEvent�� �����Ѵ�.
/// ���� ������ Save ���ְ�, Json ������ �����Ǹ� Load ���ش�.
/// </summary>
class AnimationComponent
	: public RenderComponent
{
protected:
	AnimationAsset* m_pAnimationAsset = nullptr;									// �ִϸ��̼��� ���� �⺻ ������ �ڻ� (���ο� D2D1Bitmap)

	std::wstring m_AnimationName;													// ���� �ִϸ��̼� �̸�
	size_t m_FrameIndex = 0;														// ���� ������ �ε���
	float m_ProgressTime = 0.f;														// �ִϸ��̼� ����ð�
	D2D1_RECT_F m_SrcRect = { 0.f, 0.f, 0.f, 0.f };			// �ð��� ���� D2D1Bitmap�� Source ����
	D2D1_RECT_F m_DstRect = { 0.f, 0.f, 0.f, 0.f };			// �ð��� ���� D2D1Bitmap�� Source ����
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