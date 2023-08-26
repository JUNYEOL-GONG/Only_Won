#pragma once

#include "Object.h"

class GameObject;

/// <summary>
/// ���� ������ Component
/// Component�� ���� ������ �θ� ������Ʈ, �̸��� ������.
/// �뷫���� Component ������ �Ʒ��� ����.
/// Component
///   -SceneComponent
///     -CameraComponent
///     -RenderComponent
///       -RenderableComponents...
///   -MoveComponent
///   -FSMComponent
/// </summary>
class Component
	: public Object
{
protected:
	GameObject* m_pOwner;
	std::wstring m_name;

public:
	virtual ~Component() { }

public:
	void SetOwner(GameObject* pOwner) { m_pOwner = pOwner; }
	GameObject* GetOwner() const { return m_pOwner; }

	void SetName(const std::wstring name) { m_name = name; }
	std::wstring GetName() const { return m_name; }

public:
	virtual void Update(float deltaTime) abstract;
};

