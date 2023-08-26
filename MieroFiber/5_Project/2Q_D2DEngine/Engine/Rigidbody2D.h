#pragma once

#include "Component.h"

/// <summary>
/// ��ü�� �������� Ư���� ��Ÿ���� ������Ʈ
/// </summary>
class Rigidbody2D :
    public Component
{
private:
    float m_Mass = 1.f;
    float m_Drag = 0.f;

    framework::Vector2D m_Velocity = { 0.f, 0.f };

};