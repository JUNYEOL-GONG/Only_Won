#pragma once

#include <iostream>

// ��� ����
#define FRAMEWORK_DEBUG
//#define FRAMEWORK_RELEASE

// ȭ�� ũ�� ����
#define ScreenWidth (1920)
#define ScreenHeight (1080)

// �ִ� ���� ��
#define MaxVibrationValue (65535)
#define MinVibrationValue (0)

// �α׿� ��ũ�� �Լ�
#define PrintLog(message) std::cout << "FILE : " << __FILE__ << " || LINENO : " << __LINE__ << "\n#MESSAGE : " << message << "\n\n"

#define ItemScale (40)