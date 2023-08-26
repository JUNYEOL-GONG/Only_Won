#pragma once

/// <summary>
/// Resource 사용 빈도를 나타내는 카운터.
/// 같은 리소스에 대해 참조횟수를 보관하고, RefCnt가 0이 될 경우 리소스를 해제한다.
/// </summary>
class ReferenceCounter
{
private:
	unsigned int m_RefCount;

public:
	ReferenceCounter() : m_RefCount(0) { }

	// virtual 키워드 사용으로 파괴자를 가상함수로 만들어 delete 를 호출할때 
	// 파괴자를 호출이 자식의 파괴자 -> 부모의 파괴자 순으로 호출 되도록 한다.
	// 자식클래스에서 제거한 상태로 재정의 해도 virtual 상태를 유지한다
	virtual ~ReferenceCounter() { }

	unsigned int AddRef()
	{
		m_RefCount++;
		return m_RefCount;
	}

	unsigned int Release()
	{
		m_RefCount--;
		if (m_RefCount == 0)
		{
			// 파괴자에 virtual 키워드를 사용했으므로 자식클래스의 파괴자가 호출된다.
			delete this;
			return 0;
		}
		return m_RefCount;
	}

	unsigned int GetRefCount() const
	{
		return m_RefCount;
	}
};

