#pragma once

/// <summary>
///	�ν��Ͻ�ȭ�� FSMTransition�� ����
///	��ȯ ������ �ش� Functor�� �����Ѵ�.
///	</summary>
class FSMTransition
{
public:
	virtual std::pair<std::wstring, framework::Vector2D> operator()() abstract;
};