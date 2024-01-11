#pragma once

class Event;

/// <summary>
///	이벤트 리스너로 등록할 객체는 해당 클래스를 상속받아 HandleEvent를 구현한다.
///	EventManager::Update에서 이벤트를 Execute하면 타입과 객체에 따라 HandleEvent가 실행되어 이벤트를 처리한다.
/// </summary>
class EventListener
{
public:
	virtual ~EventListener() = default;

	virtual void HandleEvent(Event* event) abstract;
};