#pragma once

#include "SceneComponent.h"

/// <summary>
/// - Legacy Version -
/// ī�޶� ������Ʈ. ī�޶� ������Ʈ�� �������� ������ �ۼ��ߴ�.
/// LateUpdate�� ī�޶� Ʈ�������� ������� �� �� ���Ѵ�.
/// �� �ϳ� �����ϴ� ī�޶� Ʈ�������� RenderComponent���� ���� ���� friend Class�� �����Ѵ�.
///
/// - Current Version -
/// Only Won���� ī�޶� ������ ����ϱ� ������ �ټ� �����ߴ� ������ ����ȭ�� �������� �����ϰ�, ����� �ش� ������Ʈ�� ������ �ʴ´�.
/// ���� ī�޶� ����� �ʿ��� ��� ī�޶� ������Ʈ�� CameraComponent + BoxCollider�� �ٿ� ������ ����ȭ�� �ٽ� ������ �� �ִ�.
/// </summary>
class CameraComponent
	: public SceneComponent
{
public:
    static D2D_MATRIX_3X2_F m_CameraTransform;

public:
    void InvertCameraTransform() { D2D1InvertMatrix(&m_CameraTransform); }

    bool Initialize() override;
    void Update(const float deltaTime) override;

    friend class RenderComponent;
};

