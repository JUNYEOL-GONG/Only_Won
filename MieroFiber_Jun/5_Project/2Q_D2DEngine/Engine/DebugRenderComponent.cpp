#include "pch.h"
#include "DebugRenderComponent.h"

#include "CommonApp.h"
#include "GameObject.h"

#include "../GameApp/TrainSpawner.h"

void DebugRenderComponent::Render(ID2D1RenderTarget* pRenderTarget)
{
	m_FinalTransform = m_WorldTransform * GetCameraTransform();
	pRenderTarget->SetTransform(m_FinalTransform);

	TrainSpawner* trainSpawner = dynamic_cast<TrainSpawner*>(m_pOwner);

	FLOAT posX = 0.f;

	if (trainSpawner->GetName() == L"TrainSpawner_1")
		posX = 1300.f;
	else if (trainSpawner->GetName() == L"TrainSpawner_2")
		posX = 1900;
	else if (trainSpawner->GetName() == L"TrainSpawner_3")
		posX = 2500;

	if(trainSpawner->m_ElapsedTriggerTime > trainSpawner->m_TriggerTime)
	{
		SetColor(D2D1::ColorF::Red);
		CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);
	}
	else
	{
		SetColor(D2D1::ColorF::Black);
		CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);
	}

	pRenderTarget->DrawTextW(
		std::to_wstring(trainSpawner->m_ElapsedTriggerTime).c_str(),
		static_cast<UINT32>(std::to_wstring(trainSpawner->m_ElapsedTriggerTime).length()),
		CommonApp::m_pInstance->GetTextFormat(),
		D2D1::RectF(0, 0, posX, 250),
		CommonApp::m_pInstance->GetBrush()
	);

	if (trainSpawner->m_ElapsedDelayTime > trainSpawner->m_DelayTime)
	{
		SetColor(D2D1::ColorF::Red);
		CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);
	}
	else
	{
		SetColor(D2D1::ColorF::Black);
		CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);
	}

	pRenderTarget->DrawTextW(
		std::to_wstring(trainSpawner->m_ElapsedDelayTime).c_str(),
		static_cast<UINT32>(std::to_wstring(trainSpawner->m_ElapsedDelayTime).length()),
		CommonApp::m_pInstance->GetTextFormat(),
		D2D1::RectF(0, 0, posX, 350),
		CommonApp::m_pInstance->GetBrush()
	);

	if (trainSpawner->m_ElapsedSpawnTime > trainSpawner->m_SpawnTime)
	{
		SetColor(D2D1::ColorF::Red);
		CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);
	}
	else
	{
		SetColor(D2D1::ColorF::Black);
		CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);
	}

	pRenderTarget->DrawTextW(
		std::to_wstring(trainSpawner->m_ElapsedSpawnTime).c_str(),
		static_cast<UINT32>(std::to_wstring(trainSpawner->m_ElapsedSpawnTime).length()),
		CommonApp::m_pInstance->GetTextFormat(),
		D2D1::RectF(0, 0, posX, 450),
		CommonApp::m_pInstance->GetBrush()
	);

	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	// 돈 운반하는 목적지의 범위
	SetColor(D2D1::ColorF::Green);
	CommonApp::m_pInstance->GetBrush()->SetColor(m_Color);

	pRenderTarget->DrawEllipse(
		D2D1::Ellipse(
			D2D1::Point2F(1650.f, 540.f),
			120.f,
			120.f
		),
		CommonApp::m_pInstance->GetBrush()
	);
}