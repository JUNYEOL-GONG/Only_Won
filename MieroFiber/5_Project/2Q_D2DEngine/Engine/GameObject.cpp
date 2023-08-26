#include "pch.h"
#include "GameObject.h"

#include "CommonApp.h"
#include "RenderComponent.h"
#include "BoxCollider2D.h"
#include "DebugRenderComponent.h"

#include "../GameApp/PlayerObject.h"	// 허허...

GameObject::~GameObject()
{
	for (const auto& component : m_OwnedComponent)
	{
		delete component;
	}
	m_OwnedComponent.clear();
}

void GameObject::SetGameObjectScale(const float x, const float y) const
{
	m_pRootComponent->SetRelativeScale(x, y);
}

void GameObject::SetGameObjectScale(framework::Vector2D vec) const
{
	m_pRootComponent->SetRelativeScale(vec);
}

framework::Vector2D GameObject::GetGameObjectScale() const
{
	return m_pRootComponent->GetRelativeRotation();
}

void GameObject::SetGameObjectRotation(const float angle) const
{
	m_pRootComponent->SetRelativeRotation(angle);
}

float GameObject::GetGameObjectRotation() const
{
	return m_pRootComponent->GetRelativeRotation();
}

void GameObject::SetGameObjectLocation(const float x, const float y) const
{
	m_pRootComponent->SetRelativeLocation(x, y);
}

framework::Vector2D GameObject::GetGameObjectLocation() const
{
	return m_pRootComponent->GetRelativeLocation();
}

void GameObject::SetWorldTransform(D2D_MATRIX_3X2_F transform)
{
	m_pRootComponent->SetWorldTransform(transform);
}

D2D_MATRIX_3X2_F GameObject::GetWorldTransform()
{
	return m_pRootComponent->GetWorldTransform();
}

bool GameObject::Initialize()
{
	// 게임 오브젝트가 가지고 있는 모든 컴포넌트를 순회
	for (const auto& component : m_OwnedComponent)
	{
		// SceneComponent라면 Initialize()를 호출한다
		SceneComponent* sceneComponent = dynamic_cast<SceneComponent*>(component);
		if (sceneComponent != nullptr)
		{
			bool res = sceneComponent->Initialize();
			assert(res);
		}
	}

	return true;
}

void GameObject::Update(const float deltaTime)
{
	for (const auto& component : m_OwnedComponent)
	{
		component->Update(deltaTime);
	}
}

void GameObject::Render()
{
	// 너구리 앞에 볼 때.... 애니메이션 먼저 렌더하게 하려고..랑 그림자 먼저 렌더
	if(dynamic_cast<PlayerObject*>(this) && dynamic_cast<PlayerObject*>(this)->GetLookFront())
	{
		RenderComponent* racconRender = nullptr;
		RenderComponent* bundleRender = nullptr;

		for (auto component : m_OwnedComponent)
		{
			RenderComponent* renderComponent = dynamic_cast<RenderComponent*>(component);
			if (component->GetName() == L"AnimationComponent")
			{
				racconRender = renderComponent;
				continue;
			}
			if (component->GetName() == L"BundleTexture")
			{
				bundleRender = renderComponent;
				continue;
			}
			if(component->GetName() == L"ShadowTexture")
			{
				if (renderComponent != nullptr && renderComponent->GetVisible())
				{
					renderComponent->Render(CommonApp::m_pInstance->GetRenderTarget());
					bundleRender->Render(CommonApp::m_pInstance->GetRenderTarget());
					racconRender->Render(CommonApp::m_pInstance->GetRenderTarget());
				}
				continue;
			}


			if (renderComponent != nullptr && renderComponent->GetVisible())
			{
				/// 디버그모드 출력을 하지 않을 경우
				BoxCollider2D* colliderComponent = dynamic_cast<BoxCollider2D*>(renderComponent);
				if (!InputManager::GetInstance()->IsDebugRendering()
					&& colliderComponent)
				{
					continue;
				}

				renderComponent->Render(CommonApp::m_pInstance->GetRenderTarget());
			}
		}
		return;
	}

	// 그냥 플레이어오브젝트일 때 그림자 먼저 렌더되게
	if(dynamic_cast<PlayerObject*>(this))
	{
		RenderComponent* racconRender = nullptr;
		RenderComponent* bundleRender = nullptr;

		for (auto component : m_OwnedComponent)
		{
			RenderComponent* renderComponent = dynamic_cast<RenderComponent*>(component);
			if (component->GetName() == L"AnimationComponent")
			{
				racconRender = renderComponent;
				continue;
			}
			if (component->GetName() == L"BundleTexture")
			{
				bundleRender = renderComponent;
				continue;
			}
			if (component->GetName() == L"ShadowTexture")
			{
				if (renderComponent != nullptr && renderComponent->GetVisible())
				{
					renderComponent->Render(CommonApp::m_pInstance->GetRenderTarget());
					racconRender->Render(CommonApp::m_pInstance->GetRenderTarget());
					bundleRender->Render(CommonApp::m_pInstance->GetRenderTarget());
				}
				continue;
			}

			if (renderComponent != nullptr && renderComponent->GetVisible())
			{
				/// 디버그모드 출력을 하지 않을 경우
				BoxCollider2D* colliderComponent = dynamic_cast<BoxCollider2D*>(renderComponent);
				if (!InputManager::GetInstance()->IsDebugRendering()
					&& colliderComponent)
				{
					continue;
				}

				renderComponent->Render(CommonApp::m_pInstance->GetRenderTarget());
			}
		}
		return;
	}

	// RenderComponent로 타입캐스팅이 되는(직/간접적으로 상속받은) 컴포넌트만 렌더링한다.
	for (auto component : m_OwnedComponent)
	{
		RenderComponent* renderComponent = dynamic_cast<RenderComponent*>(component);

		if (renderComponent != nullptr && renderComponent->GetVisible())
		{
			/// 디버그모드 출력을 하지 않을 경우
			///	1. Collider
			BoxCollider2D* colliderComponent = dynamic_cast<BoxCollider2D*>(renderComponent);
			if (!InputManager::GetInstance()->IsDebugRendering()
				&& colliderComponent)
			{
				continue;
			}

			/// 2. DebugRenderComponent
			DebugRenderComponent* debugComponent = dynamic_cast<DebugRenderComponent*>(renderComponent);
			if (!InputManager::GetInstance()->IsDebugRendering()
				&& debugComponent)
			{
				continue;
			}

			renderComponent->Render(CommonApp::m_pInstance->GetRenderTarget());
		}
	}
}

Component* GameObject::GetComponent(const std::wstring& name) const
{
	for (const auto& component : m_OwnedComponent)
	{
		if (component->GetName() == name)
			return component;
	}

	return nullptr;
}

std::vector<Component*> GameObject::GetComponents(const std::wstring& name) const
{
	std::vector<Component*> components;
	for (const auto& component : m_OwnedComponent)
	{
		if (component->GetName() == name)
			components.push_back(component);
	}

	return components;
}