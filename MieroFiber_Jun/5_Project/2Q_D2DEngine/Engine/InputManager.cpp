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

	// ��Ʈ�ѷ� ��ư ����Ʈ �ʱ�ȭ
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
	// �����찡 ��Ŀ���Ǿ����� ������ �Է��� ó������ �ʴ´�
	const HWND hWnd = GetFocus();
	if (hWnd == nullptr)
		return;

	POINT curMousePos;
	GetCursorPos(&curMousePos); // temp

	if (ScreenToClient(CommonApp::m_hWnd, &curMousePos) != false)
		m_mousePos = curMousePos;


	/// Ű���� �Է� ó��

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

	/// ��Ʈ�ѷ� �Է� ó��

	// ����� ��Ʈ�ѷ� ���� ��������
	const DWORD controllerCount = GetMaxControllerCount();

	// ��Ʈ�ѷ� ���� ��ŭ �ݺ�
	for (DWORD i = 0; i < controllerCount; i++)
	{
		// �ش� �ε����� ��Ʈ�ѷ� ���� ��������
		const XINPUT_STATE controller = GetControllerState(i);

		// ��Ʈ�ѷ��� ��� ��ư�� ���¸� üũ
		for (size_t j = 0; j < m_PadButtonList.size(); j++)
		{
			// ���� �е� �Է� ó��

			// ��ư�� ���ȴ�
			if(m_PadButtonList[j] & controller.Gamepad.wButtons)
			{
				// ��ư�� �����־���
				if(m_PadState[i][j].prevPushed)
					m_PadState[i][j].padState = eKeyState::HOLD;
				// ��ư�� �������� �ʾҴ�
				else
				{
					m_PadState[i][j].padState = eKeyState::PUSH;
					m_PadState[i][j].prevPushed = true;
				}
			}
			// ��ư�� ������ �ʾҴ�
			else
			{
				// ��ư�� �����־���
				if (m_PadState[i][j].prevPushed)
				{
					m_PadState[i][j].padState = eKeyState::END;
					m_PadState[i][j].prevPushed = false;
				}
				// ��ư�� �������� �ʾҴ�
				else
					m_PadState[i][j].padState = eKeyState::NONE;
			}
		}

		/// ��Ʈ�ѷ��� Left, Right Trigger�� ��ưȭ

		// ���� Ʈ���Ű� ���ȴ�
		if (IsTriggerLeft(i))
		{
			// �̹� �����־��ٸ�
			if(m_LeftTriggerState[i].prevPushed)
				m_LeftTriggerState[i].padState = eKeyState::HOLD;
			// �������� �ʾҴ�
			else
			{
				m_LeftTriggerState[i].padState = eKeyState::PUSH;
				m_LeftTriggerState[i].prevPushed = true;
			}
		}
		// ���� Ʈ���Ű� ������ �ʾҴ�
		else
		{
			// �̹� �����־���
			if (m_LeftTriggerState[i].prevPushed)
			{
				m_LeftTriggerState[i].padState = eKeyState::END;
				m_LeftTriggerState[i].prevPushed = false;
			}
			// �������� �ʾҴ�
			else
				m_LeftTriggerState[i].padState = eKeyState::NONE;
		}

		// ������ Ʈ���Ű� ���ȴ�
		if (IsTriggerRight(i))
		{
			// �̹� �����־��ٸ�
			if (m_RightTriggerState[i].prevPushed)
				m_RightTriggerState[i].padState = eKeyState::HOLD;
			// �������� �ʾҴ�
			else
			{
				m_RightTriggerState[i].padState = eKeyState::PUSH;
				m_RightTriggerState[i].prevPushed = true;
			}
		}
		// ������ Ʈ���Ű� ������ �ʾҴ�
		else
		{
			// �̹� �����־���
			if (m_RightTriggerState[i].prevPushed)
			{
				m_RightTriggerState[i].padState = eKeyState::END;
				m_RightTriggerState[i].prevPushed = false;
			}
			// �������� �ʾҴ�
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
		// LEFT�� �����ִ�
		if (m_KeyState[static_cast<int>(eKeyCode::LEFT)].keyState == eKeyState::HOLD &&
			m_KeyState[static_cast<int>(eKeyCode::RIGHT)].keyState != eKeyState::HOLD)
			value = -1.f;
		// RIGHT�� �����ִ�
		else if (m_KeyState[static_cast<int>(eKeyCode::RIGHT)].keyState == eKeyState::HOLD &&
			m_KeyState[static_cast<int>(eKeyCode::LEFT)].keyState != eKeyState::HOLD)
			value = 1.f;
		// �� �� �����ְų� �� �� �� �����ִ�
		else
			value = 0.f;
	}

	else if ("Vertical" == axis)
	{
		// UP�� �����ִ�
		if (m_KeyState[static_cast<int>(eKeyCode::UP)].keyState == eKeyState::HOLD &&
			m_KeyState[static_cast<int>(eKeyCode::DOWN)].keyState != eKeyState::HOLD)
			value = -1.f;
		// DOWN�� �����ִ�
		else if (m_KeyState[static_cast<int>(eKeyCode::DOWN)].keyState == eKeyState::HOLD &&
			m_KeyState[static_cast<int>(eKeyCode::UP)].keyState != eKeyState::HOLD)
			value = 1.f;
		// �� �� �����ְų� �� �� �� �����ִ�
		else
			value = 0.f;
	}

	else if ("A2D" == axis)
	{
		// A�� �����ִ�
		if (m_KeyState[static_cast<int>(eKeyCode::A)].keyState == eKeyState::HOLD &&
			m_KeyState[static_cast<int>(eKeyCode::D)].keyState != eKeyState::HOLD)
			value = -1.f;
		// D�� �����ִ�
		else if (m_KeyState[static_cast<int>(eKeyCode::D)].keyState == eKeyState::HOLD &&
			m_KeyState[static_cast<int>(eKeyCode::A)].keyState != eKeyState::HOLD)
			value = 1.f;
		// �� �� �����ְų� �� �� �� �����ִ�
		else
			value = 0.f;
	}

	else if ("W2S" == axis)
	{
		// W�� �����ִ�
		if (m_KeyState[static_cast<int>(eKeyCode::W)].keyState == eKeyState::HOLD &&
			m_KeyState[static_cast<int>(eKeyCode::S)].keyState != eKeyState::HOLD)
			value = -1.f;
		// S�� �����ִ�
		else if (m_KeyState[static_cast<int>(eKeyCode::S)].keyState == eKeyState::HOLD &&
			m_KeyState[static_cast<int>(eKeyCode::W)].keyState != eKeyState::HOLD)
			value = 1.f;
		// �� �� �����ְų� �� �� �� �����ִ�
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