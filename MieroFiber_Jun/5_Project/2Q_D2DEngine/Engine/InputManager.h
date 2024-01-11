#pragma once

#include <Xinput.h>
#include <vector>

enum class eKeyState
{
	NONE,
	PUSH,
	HOLD,
	END
};

enum eKeyCode
{
	LBUTTON = VK_LBUTTON, // 왼쪽 마우스 버튼
	RBUTTON = VK_RBUTTON, // 오른쪽 마우스 버튼
	CANCEL = VK_CANCEL,   // 취소 키
	MBUTTON = VK_MBUTTON, // 가운데 마우스 버튼

	BACK = VK_BACK,       // 백스페이스 키
	TAB = VK_TAB,		  // 탭 키
	CLEAR = VK_CLEAR,	  // CLEAR 키
	RETURN = VK_RETURN,	  // 엔터 키

	SHIFT = VK_SHIFT,     // Shift 키
	CONTROL = VK_CONTROL, // Ctrl 키
	ALT = VK_MENU,		  // Alt 키
	PAUSE = VK_PAUSE,	  // Pause 키

	ESCAPE = VK_ESCAPE,   // Esc 키

	SPACE = VK_SPACE,     // 스페이스바 키
	LEFT = VK_LEFT,		  // 왼쪽 화살표 키
	UP = VK_UP,			  // 위쪽 화살표 키
	RIGHT = VK_RIGHT,	  // 오른쪽 화살표 키
	DOWN = VK_DOWN,		  // 아래쪽 화살표 키

	F5 = VK_F5,			  // 디버깅 출력

	A = 65,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
};

struct KeyInfo
{
	bool prevPushed;
	eKeyState keyState;
};

enum class GamePadButtonCode
{
	A = 0,
	B,
	X,
	Y,

	UP,
	DOWN,
	LEFT,
	RIGHT,

	START,
	BACK,

	L3,
	R3,

	L1,
	R1,

	XINPUT_BUTTON_COUNT
};

struct GamePadButtonInfo
{
	bool prevPushed;
	eKeyState padState;
};

class InputManager
{
public:
	InputManager();

public:
	static InputManager* GetInstance()
	{
		if (m_pInputInstance == nullptr)
			m_pInputInstance = new InputManager;

		return m_pInputInstance;
	}

	void Initialize();
	void Update();
	void Finalize();

public:
	inline bool IsKeyNone(eKeyCode keyCode) const;
	inline bool IsKeyPush(eKeyCode keyCode) const;
	inline bool IsKeyHold(eKeyCode keyCode) const;
	inline bool IsKeyEnd(eKeyCode keyCode) const;

	inline const POINT& GetMousePos() const;

public:
	float GetAxisRaw(const std::string& axis) const;

public:
	static framework::Vector2D GetPadAxisLeftThumb(DWORD controllerIndex);
	static framework::Vector2D GetPadAxisRightThumb(DWORD controllerIndex);
	static float GetPadAxisLeftTrigger(DWORD controllerIndex);
	static float GetPadAxisRightTrigger(DWORD controllerIndex);
	static bool IsTriggerLeft(DWORD controllerIndex);
	static bool IsTriggerRight(DWORD controllerIndex);

private:
	static InputManager* m_pInputInstance;
	KeyInfo m_KeyState[256];
	POINT m_mousePos;

private:
	// HMODULE hX1nput;
	// typedef void (*MyFunctionType)();
	// MyFunctionType MyFunction;

private:
	static bool m_bDebugRendering;

public:
	static bool IsDebugRendering() { return m_bDebugRendering; }

private:
	std::vector<int> m_PadButtonList;
	GamePadButtonInfo m_PadState[4][static_cast<int>(GamePadButtonCode::XINPUT_BUTTON_COUNT)];

	GamePadButtonInfo m_LeftTriggerState[4];
	GamePadButtonInfo m_RightTriggerState[4];

	/// <summary>
	/// XBOX XInputTestFunc Function
	/// </summary>
public:
	inline bool IsPadButtonNone(DWORD controllerIndex, GamePadButtonCode buttonCode) const;
	inline bool IsPadButtonPush(DWORD controllerIndex, GamePadButtonCode buttonCode) const;
	inline bool IsPadButtonHold(DWORD controllerIndex, GamePadButtonCode buttonCode) const;
	inline bool IsPadButtonEnd(DWORD controllerIndex, GamePadButtonCode buttonCode) const;
	inline bool IsLeftTriggerNone(DWORD controllerIndex) const;
	inline bool IsLeftTriggerPush(DWORD controllerIndex) const;
	inline bool IsLeftTriggerHold(DWORD controllerIndex) const;
	inline bool IsLeftTriggerEnd(DWORD controllerIndex) const;
	inline bool IsRightTriggerNone(DWORD controllerIndex) const;
	inline bool IsRightTriggerPush(DWORD controllerIndex) const;
	inline bool IsRightTriggerHold(DWORD controllerIndex) const;
	inline bool IsRightTriggerEnd(DWORD controllerIndex) const;

	// 컨트롤러 버튼 상태
	static XINPUT_STATE GetControllerState(DWORD controllerIndex)
	{
		XINPUT_STATE controllerState;
		ZeroMemory(&controllerState, sizeof(XINPUT_STATE));

		// 해당 인덱스의 컨트롤러의 상태 가져오기
		XInputGetState(controllerIndex, &controllerState);
		return controllerState;
	}

	// 컨트롤러 진동 기능
	static XINPUT_VIBRATION SetControllerVib(DWORD controllerIndex, WORD leftMotorSpeed, WORD rightMotorSpeed)
	{
		XINPUT_VIBRATION vibration;
		ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

		// 최대값은 65535
		vibration.wLeftMotorSpeed = leftMotorSpeed;  // 왼쪽 모터의 진동 속도
		vibration.wRightMotorSpeed = rightMotorSpeed; // 오른쪽 모터의 진동 속도

		// 해당 인덱스의 컨트롤러에 진동을 적용
		XInputSetState(controllerIndex, &vibration);
		return vibration;
	}

	// TODO : 모든 컨드롤러 진동 기능
	static void SetAllControllerVib(WORD leftMotorSpeed, WORD rightMotorSpeed)
	{
		for (DWORD i = 0; i < GetMaxControllerCount(); i++)
		{
			XINPUT_VIBRATION vibration;
			ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

			// 최대값은 65535
			vibration.wLeftMotorSpeed = leftMotorSpeed;  // 왼쪽 모터의 진동 속도
			vibration.wRightMotorSpeed = rightMotorSpeed; // 오른쪽 모터의 진동 속도

			// 해당 인덱스의 컨트롤러에 진동을 적용
			XInputSetState(i, &vibration);
		}
	}

	// 연결되어있는 컨트롤러의 개수 가져오기
	static DWORD GetMaxControllerCount()
	{
		DWORD count = 0;
		for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
		{
			XINPUT_STATE controllerState;
			ZeroMemory(&controllerState, sizeof(XINPUT_STATE));
			if (XInputGetState(i, &controllerState) == ERROR_SUCCESS)
				count++;
		}
		return count;
	}
};

bool InputManager::IsKeyNone(eKeyCode keyCode) const
{
	return m_KeyState[keyCode].keyState == eKeyState::NONE;
}

bool InputManager::IsKeyPush(eKeyCode keyCode) const
{
	return m_KeyState[keyCode].keyState == eKeyState::PUSH;
}

bool InputManager::IsKeyHold(eKeyCode keyCode) const
{
	return m_KeyState[keyCode].keyState == eKeyState::HOLD;
}

bool InputManager::IsKeyEnd(eKeyCode keyCode) const
{
	return m_KeyState[keyCode].keyState == eKeyState::END;
}

bool InputManager::IsPadButtonNone(DWORD controllerIndex, GamePadButtonCode buttonCode) const
{
	return m_PadState[controllerIndex][static_cast<int>(buttonCode)].padState == eKeyState::NONE;
}

bool InputManager::IsPadButtonPush(DWORD controllerIndex, GamePadButtonCode buttonCode) const
{
	return m_PadState[controllerIndex][static_cast<int>(buttonCode)].padState == eKeyState::PUSH;
}

bool InputManager::IsPadButtonHold(DWORD controllerIndex, GamePadButtonCode buttonCode) const
{
	return m_PadState[controllerIndex][static_cast<int>(buttonCode)].padState == eKeyState::HOLD;
}

bool InputManager::IsPadButtonEnd(DWORD controllerIndex, GamePadButtonCode buttonCode) const
{
	return m_PadState[controllerIndex][static_cast<int>(buttonCode)].padState == eKeyState::END;
}

bool InputManager::IsLeftTriggerNone(DWORD controllerIndex) const
{
	return m_LeftTriggerState[controllerIndex].padState == eKeyState::NONE;
}

bool InputManager::IsLeftTriggerPush(DWORD controllerIndex) const
{
	return m_LeftTriggerState[controllerIndex].padState == eKeyState::PUSH;
}

bool InputManager::IsLeftTriggerHold(DWORD controllerIndex) const
{
	return m_LeftTriggerState[controllerIndex].padState == eKeyState::HOLD;
}

bool InputManager::IsLeftTriggerEnd(DWORD controllerIndex) const
{
	return m_LeftTriggerState[controllerIndex].padState == eKeyState::END;
}

bool InputManager::IsRightTriggerNone(DWORD controllerIndex) const
{
	return m_RightTriggerState[controllerIndex].padState == eKeyState::NONE;
}

bool InputManager::IsRightTriggerPush(DWORD controllerIndex) const
{
	return m_RightTriggerState[controllerIndex].padState == eKeyState::PUSH;
}

bool InputManager::IsRightTriggerHold(DWORD controllerIndex) const
{
	return m_RightTriggerState[controllerIndex].padState == eKeyState::HOLD;
}

bool InputManager::IsRightTriggerEnd(DWORD controllerIndex) const
{
	return m_RightTriggerState[controllerIndex].padState == eKeyState::END;
}

const POINT& InputManager::GetMousePos() const
{
	return m_mousePos;
}