#pragma once

/// <summary>
///	이벤트에 추가적으로 Functor가 필요한 경우를 위해 만들었다.
///	현재는 사용하지 않는다.(23.08.10)
/// </summary>
struct EventFunctor
{
	virtual void operator()() abstract;
};