#pragma once

class Event;

/// <summary>
///	�̺�Ʈ �����ʷ� ����� ��ü�� �ش� Ŭ������ ��ӹ޾� HandleEvent�� �����Ѵ�.
///	EventManager::Update���� �̺�Ʈ�� Execute�ϸ� Ÿ�԰� ��ü�� ���� HandleEvent�� ����Ǿ� �̺�Ʈ�� ó���Ѵ�.
/// </summary>
class EventListener
{
public:
	virtual ~EventListener() = default;

	virtual void HandleEvent(Event* event) abstract;
};