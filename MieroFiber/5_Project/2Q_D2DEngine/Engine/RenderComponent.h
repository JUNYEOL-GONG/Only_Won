#pragma once

#include "SceneComponent.h"
#include "CameraComponent.h"

/// <summary>
/// �������� �ʿ��� ������Ʈ�� �ش� Ŭ������ ��ӹ޾� �����Ѵ�.
/// �ν��Ͻ�ȭ���� �ʴ´�.
/// </summary>
class RenderComponent
	: public SceneComponent
{
protected:
	int m_RenderOrder = 0;
	bool m_bVisible = true;

public:
	const D2D_MATRIX_3X2_F GetCameraTransform() const { return CameraComponent::m_CameraTransform; }

	void SetVisible(bool isVisible) { m_bVisible = isVisible; }
	bool GetVisible() { return m_bVisible; }

public:
	virtual void Render(ID2D1RenderTarget* pRenderTarget) abstract;
};

