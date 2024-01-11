#pragma once

#include "SceneComponent.h"

/// <summary>
/// - Legacy Version -
/// 카메라 컴포넌트. 카메라 오브젝트가 존재함을 전제로 작성했다.
/// LateUpdate에 카메라 트랜스폼의 역행렬을 한 번 구한다.
/// 단 하나 존재하는 카메라 트랜스폼을 RenderComponent에서 쓰기 위해 friend Class로 선언한다.
///
/// - Current Version -
/// Only Won에서 카메라 고정을 사용하기 때문에 다소 복잡했던 렌더링 최적화를 구조에서 제거하고, 현재는 해당 컴포넌트가 사용되지 않는다.
/// 이후 카메라 사용이 필요할 경우 카메라 오브젝트에 CameraComponent + BoxCollider를 붙여 렌더링 최적화를 다시 구현할 수 있다.
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

