#include "pch.h"
#include "DirectionComponent.h"

#include "CommonApp.h"
#include "GameObject.h"
#include "PlayerMovement.h"
#include "TextureAsset.h"

DirectionComponent::DirectionComponent()
{
	m_pTextureAsset = new TextureAsset;

	SetTextureAsset(L"../Resource/clear.png", L"Clear");
}

DirectionComponent::~DirectionComponent()
{
	if (m_pTextureAsset != nullptr)
	{
		delete m_pTextureAsset;
		m_pTextureAsset = nullptr;
	}
}

void DirectionComponent::SetTextureAsset(const WCHAR* szFilePath, const std::wstring textureName)
{
	m_pTextureAsset->SetBitmapFilePath(szFilePath);
	m_pTextureAsset->Build();
}

void DirectionComponent::Render(ID2D1RenderTarget* pRenderTarget)
{
	// PlayerMovement 컴포넌트를 통해 이동 방향과 바라보는 방향을 가져온다.
	const framework::Vector2D moveDir = dynamic_cast<PlayerMovement*>(m_pOwner->GetComponent(L"PlayerMovement"))->GetMoveDir();
	const framework::Vector2D lookDir = dynamic_cast<PlayerMovement*>(m_pOwner->GetComponent(L"PlayerMovement"))->GetLookDir();

	/// Projectile Object Render

	// Projectile TextureAsset이 있고 Player가 Projectile Item을 가지고 있는 경우에만 Rendering
	if ((m_pTextureAsset != nullptr) && (m_HasProjectile == true))
	{
		// 바라보는 방향에 따른 Rotation 설정
		float theta = atan(lookDir.y / lookDir.x); // 아크탄젠트 함수를 사용해 theta를 구함
		theta = theta * 180.f / 3.14159265f; // 라디안을 도로 변환

		// 중심으로부터 텍스쳐 렌더 생성 Distance 설정
		constexpr float distance = 75.f;
		const framework::Vector2D distVec = lookDir * distance;

		// Image Flip 설정
		int flipX = 1;
		if (lookDir.x < 0) flipX = -1;
		else if (lookDir.x >= 0) flipX = 1;

		// 트랜스폼 설정
		const D2D_MATRIX_3X2_F tempTransform = D2D1::Matrix3x2F::Scale(D2D1::SizeF(static_cast<float>(flipX), 1.f))
			* D2D1::Matrix3x2F::Rotation(theta, { 0, 0 })
			* D2D1::Matrix3x2F::Translation(D2D1::SizeF(distVec.x, distVec.y))
			* m_WorldTransform;

		// 트랜스폼 적용
		const D2D_MATRIX_3X2_F tempFinalTransform = tempTransform * GetCameraTransform();
		pRenderTarget->SetTransform(tempFinalTransform);

		// 비트맵 사이즈
		const D2D1_SIZE_F bitmapSize = m_pTextureAsset->m_pBitmap->GetSize();

		pRenderTarget->DrawBitmap(
			m_pTextureAsset->m_pBitmap,
			{ -bitmapSize.width / 2.f, -bitmapSize.height / 2.f, bitmapSize.width / 2.f, bitmapSize.height / 2.f },
			1.f,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			{ 0.f, 0.f, bitmapSize.width, bitmapSize.height });
	}

	/*
	// 트랜스폼 설정
	m_FinalTransform = m_WorldTransform * GetCameraTransform();
	pRenderTarget->SetTransform(m_FinalTransform);

	/// Move Direction Render

	SetColor(D2D1::ColorF::Yellow);
	CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);

	// Draw Move Direction
	pRenderTarget->DrawLine(
		D2D1::Point2F(0.f, 0.f),
		D2D1::Point2F(
			50.f * moveDir.x,
			50.f * moveDir.y
		),
		CommonApp::m_pInstance->GetBrush(),
		3.f);

	/// Look Direction Render

	SetColor(D2D1::ColorF::HotPink);
	CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);

	// Draw Look Direction
	pRenderTarget->DrawLine(
		D2D1::Point2F(0.f, 0.f),
		D2D1::Point2F(
			50.f * lookDir.x,
			50.f * lookDir.y
		),
		CommonApp::m_pInstance->GetBrush(),
		3.f);
	*/

	/*
	/// Velocity Test Code

	SetColor(D2D1::ColorF::Black);
	CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);

	if (m_pOwner->GetName() == L"Player1")
	{
		const framework::Vector2D OriginalVelocity = dynamic_cast<PlayerMovement*>(m_pOwner->GetComponent(L"PlayerMovement"))->GetOriginalVelocity();
		const framework::Vector2D ExternalVelocity = dynamic_cast<PlayerMovement*>(m_pOwner->GetComponent(L"PlayerMovement"))->GetExternalVelocity();

		// const std::wstring temp1 = std::to_wstring((int)OriginalVelocity.x) + L", " + std::to_wstring((int)OriginalVelocity.y);
		// const std::wstring temp2 = std::to_wstring((int)ExternalVelocity.x) + L", " + std::to_wstring((int)ExternalVelocity.y);

		const std::wstring temp1 = std::to_wstring(moveDir.x) + L", " + std::to_wstring(moveDir.y);
		const std::wstring temp2 = std::to_wstring(lookDir.x) + L", " + std::to_wstring(lookDir.y);

		pRenderTarget->DrawTextW(
			temp1.c_str(),
			static_cast<UINT32>(temp1.length()),
			CommonApp::m_pInstance->GetTextFormat(),
			D2D1::RectF(0, 0, 100, 500),
			CommonApp::m_pInstance->GetBrush()
		);

		pRenderTarget->DrawTextW(
			temp2.c_str(),
			static_cast<UINT32>(temp2.length()),
			CommonApp::m_pInstance->GetTextFormat(),
			D2D1::RectF(0, 0, 100, 600),
			CommonApp::m_pInstance->GetBrush()
		);
	}
	else if (m_pOwner->GetName() == L"Player2")
	{
		const framework::Vector2D OriginalVelocity = dynamic_cast<PlayerMovement*>(m_pOwner->GetComponent(L"PlayerMovement"))->GetOriginalVelocity();
		const framework::Vector2D ExternalVelocity = dynamic_cast<PlayerMovement*>(m_pOwner->GetComponent(L"PlayerMovement"))->GetExternalVelocity();

		// const std::wstring temp1 = std::to_wstring((int)OriginalVelocity.x) + L", " + std::to_wstring((int)OriginalVelocity.y);
		// const std::wstring temp2 = std::to_wstring((int)ExternalVelocity.x) + L", " + std::to_wstring((int)ExternalVelocity.y);

		const std::wstring temp1 = std::to_wstring(moveDir.x) + L", " + std::to_wstring(moveDir.y);
		const std::wstring temp2 = std::to_wstring(lookDir.x) + L", " + std::to_wstring(lookDir.y);

		pRenderTarget->DrawTextW(
			temp1.c_str(),
			static_cast<UINT32>(temp1.length()),
			CommonApp::m_pInstance->GetTextFormat(),
			D2D1::RectF(0, 0, 100, 800),
			CommonApp::m_pInstance->GetBrush()
		);

		pRenderTarget->DrawTextW(
			temp2.c_str(),
			static_cast<UINT32>(temp2.length()),
			CommonApp::m_pInstance->GetTextFormat(),
			D2D1::RectF(0, 0, 100, 900),
			CommonApp::m_pInstance->GetBrush()
		);
	}
	*/

	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}
