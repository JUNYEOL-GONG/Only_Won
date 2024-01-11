#include "pch.h"
#include "EventManager.h"

#include "Event.h"
#include "EventListener.h"
#include "../GameApp/ItemObject.h"

EventManager* EventManager::m_pInstance = nullptr;

EventManager* EventManager::GetInstance()
{
	if (!m_pInstance)
		m_pInstance = new EventManager;
	return m_pInstance;
}

void EventManager::RegisterListener(eEventType eventType, EventListener* listener)
{
	// listener nullptr 확인, 이미 등록되어 있는지 확인
	assert(listener);
	if(IsRegistered(eventType, listener))
	{
		return;
	}

	m_WholeEvents.insert(std::make_pair(eventType, listener));
}

void EventManager::UnregisterListener(eEventType eventType, EventListener* listener)
{
	std::pair<std::multimap<eEventType, EventListener*>::iterator,
			  std::multimap<eEventType, EventListener*>::iterator> range;
	range = m_WholeEvents.equal_range(eventType);

	for(auto& iter = range.first; iter!=range.second; ++iter)
	{
		if(iter->second == listener)
		{
			iter = m_WholeEvents.erase(iter);
			break;
		}
	}
}

void EventManager::UnregisterAll(EventListener* listener)
{
	auto iter = m_WholeEvents.begin();

	while(iter!=m_WholeEvents.end())
	{
		if(iter->second == listener)
		{
			iter = m_WholeEvents.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void EventManager::ClearEvents()
{
	m_WholeEvents.clear();
	m_EventList.clear();
}

void EventManager::Initialize()
{
	m_bInitializing = true;

	while (!m_EventList.empty())
	{
		ExecuteEvent(&m_EventList.front());
		m_EventList.pop_front();
	}

	m_bInitializing = false;
}

void EventManager::Update()
{
	while(!m_EventList.empty())
	{
		ExecuteEvent(&m_EventList.front());
		if(m_bChangingWorld)
		{
			m_bChangingWorld = false;
			return;
		}
		m_EventList.pop_front();
	}
}

void EventManager::Finalize()
{
	delete m_pInstance;

	//m_WholeEvents.clear();
	//m_EventList.clear();
}

void EventManager::SendEvent(eEventType eventId)
{
	Event newEvent(eventId);
	m_EventList.emplace_back(newEvent);
}

void EventManager::SendEvent(eEventType eventId, framework::EVENT_ANIMATION_INFO animationInfo)
{
	Event newEvent(eventId, animationInfo);
	m_EventList.emplace_back(newEvent);
}

void EventManager::SendEvent(eEventType eventId, framework::EVENT_MOVEMENT_INFO movementInfo)
{
	Event newEvent(eventId, movementInfo);
	m_EventList.emplace_back(newEvent);
}

void EventManager::SendEvent(eEventType eventId, GROUP_TYPE group, GameObject* obj)
{
	Event newEvent(eventId, group, obj);
	m_EventList.emplace_back(newEvent);
}

void EventManager::SendEvent(eEventType eventId, UINT playerNum, eItemBoxType itemBoxType, eItemType itemType)
{
	Event newEvent(eventId, playerNum, itemBoxType, itemType);
	m_EventList.emplace_back(newEvent);
}


void EventManager::SendEvent(eEventType eventId, int hp)
{
	Event newEvent(eventId, hp);
	m_EventList.emplace_back(newEvent);
}

void EventManager::SendEvent(eEventType eventId, UINT playerNum, framework::Vector2D playerLocation, framework::Vector2D lookDirection)
{
	Event newEvent(eventId, playerNum, playerLocation, lookDirection);
	m_EventList.emplace_back(newEvent);
}

bool EventManager::IsRegistered(eEventType eventType, EventListener* listener)
{
	bool alreadyRegistered = false;

	// 해당 이벤트 타입을 키값으로 가진 최소 범위, 최대 범위를 반환
	// 탐색 범위 축소
	std::pair<std::multimap<eEventType, EventListener*>::iterator,
			  std::multimap<eEventType, EventListener*>::iterator> range;
	range = m_WholeEvents.equal_range(eventType);

	// 이미 등록된 이벤트인지 확인
	for(auto& iter = range.first; iter!=range.second; ++iter)
	{
		if(iter->second == listener)
		{
			alreadyRegistered = true;
			break;
		}
	}

	return alreadyRegistered;
}

void EventManager::ExecuteEvent(Event* event)
{
	std::pair<std::multimap<eEventType, EventListener*>::iterator,
		std::multimap<eEventType, EventListener*>::iterator> range;
	range = m_WholeEvents.equal_range(event->eventID);

	for(auto& iter = range.first; iter!=range.second; ++iter)
	{
		if(ChangeWorld_Start < (event->eventID) 
			&& ChangeWorld_End > (event->eventID))
		{
			m_bChangingWorld = true;
		}

		// 해당 객체의 HandleEvent 함수 실행
		iter->second->HandleEvent(event);

		if(!m_bInitializing && m_bChangingWorld)
		{
			return;	// P1SetDefault해주고 P2가야되는데 여기서 걸려서 나가버림
		}
	}
}
