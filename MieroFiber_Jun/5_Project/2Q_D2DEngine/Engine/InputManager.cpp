#include "pch.h"
#include "InputManager.h"
#include "CommonApp.h"

#pragma comment(lib, "Xinput.lib")

InputManager* InputManager::m_pInputInstance = nullptr;
bool InputManager::m_bDebugRendering = false;

InputManager::InputManager()
	: m_KeyState{}, m_mousePos(), m_PadState {}
{
}

void InputManager::Initialize()
{
	// X1nput
	// hX1nput = LoadLibrary(TEXT("X1nput.dll"));
	// MyFunction = reinterpret_cast<MyFunctionType>(GetProcAddress(hX1nput, "MyFunction"));

	for (int i = 0; i < 256; i++)
	{
		m_KeyState[i].keyState = eKeyState::NONE;
		m_KeyState[i].prevPushed = false;
	}

	// 컨트롤러 버튼 리스트 초기화
	m_PadButtonList.emplace_back(XINPUT_GAMEPAD_A);
	m_PadButtonList.emplace_back(XINPUT_GAMEPAD_B);
	m_PadButtonList.emplace_back(XINPUT_GAMEPAD_X);
	m_PadButtonList.emplace_back(XINPUT_GAMEPAD_Y);
	m_PadButtonList.emplace_back(XINPUT_GAMEPAD_DPAD_UP);
	m_PadButtonList.emplace_back(XINPUT_GAMEPAD_DPAD_DOWN);
	m_PadButtonList.emplace_back(XINPUT_GAMEPAD_DPAD_LEFT);
	m_PadButtonList.emplace_back(XINPUT_GAMEPAD_DPAD_RIGHT);
	m_PadButtonList.emplace_back(XINPUT_GAMEPAD_START);
	m_PadButtonList.emplace_back(XINPUT_GAMEPAD_BACK);
	m_PadButtonList.emplace_back(XINPUT_GAMEPAD_LEFT_THUMB);
	m_PadButtonList.emplace_back(XINPUT_GAMEPAD_RIGHT_THUMB);
	m_PadButtonList.emplace_back(XINPUT_GAMEPAD_LEFT_SHOULDER);
	m_PadButtonList.emplace_back(XINPUT_GAMEPAD_RIGHT_SHOULDER);
}

void InputManager::Update()
{
	// 윈도우가 포커스되어있지 않으면 입력을 처리하지 않는다
	const HWND hWnd = GetFocus();
	if (hWnd == nullptr)
		return;

	POINT curMousePos;
	GetCursorPos(&curMousePos); // temp

	if (ScreenToClient(CommonApp::m_hWnd, &curMousePos) != false)
		m_mousePos = curMousePos;


	/// 키보드 입력 처리

	for (int i = 0; i < 256; i++)
	{
		if (GetAsyncKeyState(i) & 0x8000)
		{
			if (m_KeyState[i].prevPushed)
				m_KeyState[i].keyState = eKeyState::HOLD;
			else
			{
				m_KeyState[i].keyState = eKeyState::PUSH;
				if(i == eKeyCode::F5)
				{
					m_bDebugRendering = !m_bDebugRendering;
				}
				m_KeyState[i].prevPushed = true;
			}
		}
		else
		{
			if (m_KeyState[i].prevPushed)
			{
				m_KeyState[i].keyState = eKeyState::END;
				m_KeyState[i].prevPushed = false;
			}
			else
				m_KeyState[i].keyState = eKeyState::NONE;
		}
	}

	/// 컨트롤러 입력 처리

	// 연결된 컨트롤러 개수 가져오기
	const DWORD controllerCount = GetMaxControllerCount();

	// 컨트롤러 개수 만큼 반복
	for (DWORD i = 0; i < controllerCount; i++)
	{
		// 해당 인덱스의 컨트롤러 상태 가져오기
		const XINPUT_STATE controller = GetControllerState(i);

		// 컨트롤러의 모든 버튼의 상태를 체크
		for (size_t j = 0; j < m_PadButtonList.size(); j++)
		{
			// 게임 패드 입력 처리

			// 버튼이 눌렸다
			if(m_PadButtonList[j] & controller.Gamepad.wButtons)
			{
				// 버튼이 눌려있었다
				if(m_PadState[i][j].prevPushed)
					m_PadState[i][j].padState = eKeyState::HOLD;
				// 버튼이 눌려있지 않았다
				else
				{
					m_PadState[i][j].padState = eKeyState::PUSH;
					m_PadState[i][j].prevPushed = true;
				}
			}
			// 버튼이 눌리지 않았다
			else
			{
				// 버튼이 눌려있었다
				if (m_PadState[i][j].prevPushed)
				{
					m_PadState[i][j].padState = eKeyState::END;
					m_PadState[i][j].prevPushed = false;
				}
				// 버튼이 눌려있지 않았다
				else
					m_PadState[i][j].padState = eKeyState::NONE;
			}
		}

		/// 컨트롤러의 Left, Right Trigger를 버튼화

		// 왼쪽 트리거가 눌렸다
		if (IsTriggerLeft(i))
		{
			// 이미 눌려있었다면
			if(m_LeftTriggerState[i].prevPushed)
				m_LeftTriggerState[i].padState = eKeyState::HOLD;
			// 눌려있지 않았다
			else
			{
				m_LeftTriggerState[i].padState = eKeyState::PUSH;
				m_LeftTriggerState[i].prevPushed = true;
			}
		}
		// 왼쪽 트리거가 눌리지 않았다
		else
		{
			// 이미 눌려있었다
			if (m_LeftTriggerState[i].prevPushed)
			{
				m_LeftTriggerState[i].padState = eKeyState::END;
				m_LeftTriggerState[i].prevPushed = false;
			}
			// 눌려있지 않았다
			else
				m_LeftTriggerState[i].padState = eKeyState::NONE;
		}

		// 오른쪽 트리거가 눌렸다
		if (IsTriggerRight(i))
		{
			// 이미 눌려있었다면
			if (m_RightTriggerState[i].prevPushed)
				m_RightTriggerState[i].padState = eKeyState::HOLD;
			// 눌려있지 않았다
			else
			{
				m_RightTriggerState[i].padState = eKeyState::PUSH;
				m_RightTriggerState[i].prevPushed = true;
			}
		}
		// 오른쪽 트리거가 눌리지 않았다
		else
		{
			// 이미 눌려있었다
			if (m_RightTriggerState[i].prevPushed)
			{
				m_RightTriggerState[i].padState = eKeyState::END;
				m_RightTriggerState[i].prevPushed = false;
			}
			// 눌려있지 않았다
			else
				m_RightTriggerState[i].padState = eKeyState::NONE;
		}
	}
}

void InputManager::Finalize()
{
	// FreeLibrary(hX1nput);
	delete m_pInputInstance;
}

float InputManager::GetAxisRaw(const std::string& axis) const
{
	float value = 0.f;

	if ("Horizontal" == axis)
	{
		// LEFT만 눌려있다
		if (m_KeyState[static_cast<int>(eKeyCode::LEFT)].keyState == eKeyState::HOLD &&
			m_KeyState[static_cast<int>(eKeyCode::RIGHT)].keyState != eKeyState::HOLD)
			value = -1.f;
		// RIGHT만 눌려있다
		else if (m_KeyState[static_cast<int>(eKeyCode::RIGHT)].keyState == eKeyState::HOLD &&
			m_KeyState[static_cast<int>(eKeyCode::LEFT)].keyState != eKeyState::HOLD)
			value = 1.f;
		// 둘 다 눌려있거나 둘 다 안 눌려있다
		else
			value = 0.f;
	}

	else if ("Vertical" == axis)
	{
		// UP만 눌려있다
		if (m_KeyState[static_cast<int>(eKeyCode::UP)].keyState == eKeyState::HOLD &&
			m_KeyState[static_cast<int>(eKeyCode::DOWN)].keyState != eKeyState::HOLD)
			value = -1.f;
		// DOWN만 눌려있다
		else if (m_KeyState[static_cast<int>(eKeyCode::DOWN)].keyState == eKeyState::HOLD &&
			m_KeyState[static_cast<int>(eKeyCode::UP)].keyState != eKeyState::HOLD)
			value = 1.f;
		// 둘 다 눌려있거나 둘 다 안 눌려있다
		else
			value = 0.f;
	}

	else if ("A2D" == axis)
	{
		// A만 눌려있다
		if (m_KeyState[static_cast<int>(eKeyCode::A)].keyState == eKeyState::HOLD &&
			m_KeyState[static_cast<int>(eKeyCode::D)].keyState != eKeyState::HOLD)
			value = -1.f;
		// D만 눌려있다
		else if (m_KeyState[static_cast<int>(eKeyCode::D)].keyState == eKeyState::HOLD &&
			m_KeyState[static_cast<int>(eKeyCode::A)].keyState != eKeyState::HOLD)
			value = 1.f;
		// 둘 다 눌려있거나 둘 다 안 눌려있다
		else
			value = 0.f;
	}

	else if ("W2S" == axis)
	{
		// W만 눌려있다
		if (m_KeyState[static_cast<int>(eKeyCode::W)].keyState == eKeyState::HOLD &&
			m_KeyState[static_cast<int>(eKeyCode::S)].keyState != eKeyState::HOLD)
			value = -1.f;
		// S만 눌려있다
		else if (m_KeyState[static_cast<int>(eKeyCode::S)].keyState == eKeyState::HOLD &&
			m_KeyState[static_cast<int>(eKeyCode::W)].keyState != eKeyState::HOLD)
			value = 1.f;
		// 둘 다 눌려있거나 둘 다 안 눌려있다
		else
			value = 0.f;
	}

	return value;
}

framework::Vector2D InputManager::GetPadAxisLeftThumb(DWORD controllerIndex)
{
	const XINPUT_STATE state = GetControllerState(controllerIndex);

	float thumbLX = static_cast<float>(state.Gamepad.sThumbLX) / 32767.f;
	float thumbLY = static_cast<float>(state.Gamepad.sThumbLY) / 32767.f;

	const framework::Vector2D thumbLeft = { thumbLX, thumbLY };
	if (thumbLeft.Length() < 0.8f)
	{
		thumbLX = 0.f;
		thumbLY = 0.f;
	}

	return { thumbLX, thumbLY };
}

framework::Vector2D InputManager::GetPadAxisRightThumb(DWORD controllerIndex)
{
	const XINPUT_STATE state = GetControllerState(controllerIndex);

	float thumbRX = static_cast<float>(state.Gamepad.sThumbRX) / 32767.f;
	float thumbRY = static_cast<float>(state.Gamepad.sThumbRY) / 32767.f;

	const framework::Vector2D thumbRight = { thumbRX, thumbRY };
	if (thumbRight.Length() < 0.8f)
	{
		thumbRX = 0.f;
		thumbRY = 0.f;
	}

	return framework::Vector2D(thumbRX, thumbRY).Normalize();
}

float InputManager::GetPadAxisLeftTrigger(DWORD controllerIndex)
{
	const XINPUT_STATE state = GetControllerState(controllerIndex);

	if (state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return static_cast<float>(state.Gamepad.bLeftTrigger) / 255.f;

	return 0.f;
}

float InputManager::GetPadAxisRightTrigger(DWORD controllerIndex)
{
	const XINPUT_STATE state = GetControllerState(controllerIndex);

	if (state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return static_cast<float>(state.Gamepad.bRightTrigger) / 255.f;

	return 0.f;
}

bool InputManager::IsTriggerLeft(DWORD controllerIndex)
{
	const XINPUT_STATE state = GetControllerState(controllerIndex);

	if (state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return true;

	return false;
}

bool InputManager::IsTriggerRight(DWORD controllerIndex)
{
	const XINPUT_STATE state = GetControllerState(controllerIndex);

	if (state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return true;

	return false;
}