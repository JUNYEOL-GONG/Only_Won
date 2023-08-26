#include "framework.h"
#include "BoxObject.h"

#include "../Engine/BoxComponent.h"
#include "../Engine/BoxCollider2D.h"

#include "../Engine/CommonApp.h"

bool BoxObject::Initialize()
{
	/// BoxComponent
	 m_pBoxComponent = CreateComponent<BoxComponent>(L"BoxComponent");
	 SetRootComponent(m_pBoxComponent);

	 /// BoxCollider2D
	 m_pBoxCollider2D = CreateComponent<BoxCollider2D>(L"BoxCollider2D");
	 m_pBoxCollider2D->AttachToComponent(m_pBoxComponent);

	GameObject::Initialize();

	return true;
}

void BoxObject::Update(const float deltaTime)
{
	GameObject::Update(deltaTime);
}