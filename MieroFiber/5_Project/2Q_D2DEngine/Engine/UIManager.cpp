#include "pch.h"
#include "UIManager.h"

#include "Component.h"
#include "EventManager.h"
#include "InputManager.h"
#include "SceneComponent.h"
#include "UI.h"
#include "World.h"
#include "ButtonUI.h"
#include "GameObject.h"
#include "../GameApp/PopUpUIObject.h"

#include "../GameApp/UIObject.h"

#include <algorithm>

UIManager::~UIManager()
{
}

// world ui ó�� ������ ���� �˾� �� �ߴϱ�
void UIManager::SetWorldUIObject(std::vector<GameObject*>& curObject)
{
	if (curObject.empty())
	{
		return;
	}

	std::vector<GameObject*>::iterator targetIter = curObject.end();
	std::vector<GameObject*>::iterator iter = curObject.begin();

	// �⺻ ui������Ʈ�� ���� ������Ʈ�� ����
	for (; iter != curObject.end(); ++iter)
	{
		m_pUIObject = (*iter);
		m_pCurGameObject = (*iter);
		// wprintf(L"%s", (*iter)->GetName().c_str());

	}

	const std::vector<Component*>& childComponent =
		m_pUIObject->GetRootComponent()->GetChildrenComponent();

	if (childComponent.empty())
		return;

	// UI ��ġ�� ���ߺ��ͷ� �Ű��ֱ�
	size_t capacity = dynamic_cast<UI*>(childComponent[childComponent.size() - 1])->GetYIndex();

	size_t max = 0;

	for (size_t i = 0; i < childComponent.size(); i++)
	{
		if (max < dynamic_cast<UI*>(childComponent[i])->GetXIndex())
		{
			max = dynamic_cast<UI*>(childComponent[i])->GetXIndex();
		}
	}

	m_UIObjectLocation.resize(capacity + 1, std::vector<UI*>(max + 1, nullptr));

	for (size_t i = 0; i < childComponent.size(); i++)
	{
		if (!(dynamic_cast<UI*>(childComponent[i])->IsActiveUI()))
		{
			continue;
		}
		size_t y = dynamic_cast<UI*>(childComponent[i])->GetYIndex();
		size_t x = dynamic_cast<UI*>(childComponent[i])->GetXIndex();

		m_UIObjectLocation[y][x] = dynamic_cast<UI*>(childComponent[i]);
	}

	const std::vector<std::vector<UI*>>& uiLocation = m_UIObjectLocation;
	for (auto& curUIList : uiLocation)
	{
		// ó���� Focus ������ UI�� focus���ֱ�
		for (auto& focusUI : curUIList)
		{
			if (m_pFocusedUI != nullptr)
			{
				break;
			}
			if (focusUI != nullptr)
			{
				m_curX = focusUI->GetXIndex();
				m_curY = focusUI->GetYIndex();
				m_pFocusedUI = focusUI;
				if(m_pFocusedUI->GetChildrenComponent().size() > 1)
					m_pFocusedUI->FocusOn(m_pFocusedUI->GetChildrenComponent());
				else
					m_pFocusedUI->FocusOn(D2D1::ColorF::Yellow);
				break;
			}
		}
	}

	m_finalControllerPos = { 0.f,0.f };
	m_curControllerPos = { 0.f, 0.f };
}

void UIManager::SetPopUpUIVector(GameObject* curObject)
{
	m_PopUpUIObjectLocation.clear();
	const std::vector<Component*>& childComponent =
		curObject->GetRootComponent()->GetChildrenComponent();

	size_t maxX = 0;
	size_t maxY = 0;

	for (size_t i = 0; i < childComponent.size(); i++)
	{
		if(i == 0)
		{
			maxX = dynamic_cast<UI*>(childComponent[i])->GetXIndex();
			maxY = dynamic_cast<UI*>(childComponent[i])->GetYIndex();
			continue;
		}
		if (maxX < dynamic_cast<UI*>(childComponent[i])->GetXIndex())
		{
			maxX = dynamic_cast<UI*>(childComponent[i])->GetXIndex();
		}
		if (maxY < dynamic_cast<UI*>(childComponent[i])->GetYIndex())
		{
			maxY = dynamic_cast<UI*>(childComponent[i])->GetYIndex();
		}
	}

	m_PopUpUIObjectLocation.resize(maxY + 1, std::vector<UI*>(maxX + 1, nullptr));

	/*for(size_t i = 0; i < m_PopUpUIObjectLocation.size(); i++)
	{
		for(size_t j = 0; j < m_PopUpUIObjectLocation[i].size(); j++)
		{
			m_PopUpUIObjectLocation[i][j] = nullptr;
		}
	}*/

	for (size_t i = 0; i < childComponent.size(); i++)
	{
		if (!(dynamic_cast<UI*>(childComponent[i])->IsActiveUI()))
		{
			continue;
		}
		size_t y = dynamic_cast<UI*>(childComponent[i])->GetYIndex();
		size_t x = dynamic_cast<UI*>(childComponent[i])->GetXIndex();

		m_PopUpUIObjectLocation[y][x] = dynamic_cast<UI*>(childComponent[i]);
	}
}

void UIManager::SetWorldPopUpUIObject(std::vector<GameObject*>& curObject)
{
	if (curObject.empty())
	{
		return;
	}
	//std::vector<GameObject*>::iterator targetIter = curObject.end();
	//std::vector<GameObject*>::iterator iter = curObject.begin();

	for (size_t i = 0; i < curObject.size(); i++)
	{
		std::vector<Component*>& childComponent =
			curObject[i]->GetRootComponent()->GetChildrenComponent();

		if (childComponent.empty())
			continue;

		m_pPopUpUIObject.push_back(curObject[i]);
	}

	// popupui�� �����صα�
	//for (; iter != curObject.end(); ++iter)
	//{
	//	std::vector<Component*>& childComponent =
	//		(*iter)->GetRootComponent()->GetChildrenComponent();

	//	if (childComponent.size() == 0)
	//		continue;

	//	// UI ��ġ�� ���ߺ��ͷ� �Ű��ֱ�
	//	size_t capacity = dynamic_cast<UI*>(childComponent[childComponent.size() - 1])->GetYIndex();

	//	size_t max = 0;

	//	for (size_t i = 0; i < childComponent.size(); i++)
	//	{
	//		if (max < dynamic_cast<UI*>(childComponent[i])->GetXIndex())
	//		{
	//			max = dynamic_cast<UI*>(childComponent[i])->GetXIndex();
	//		}
	//	}

	//	m_PopUpUIObjectLocation.resize(capacity + 1, std::vector<UI*>(max + 1, nullptr));

	//	for (size_t i = 0; i < childComponent.size(); i++)
	//	{
	//		size_t y = dynamic_cast<UI*>(childComponent[i])->GetYIndex();
	//		size_t x = dynamic_cast<UI*>(childComponent[i])->GetXIndex();

	//		m_PopUpUIObjectLocation[y][x] = dynamic_cast<UI*>(childComponent[i]);
	//	}

	//	m_pPopUpUIObject.push_back(*iter);
	//}
}

void UIManager::Update(float deltaTime)
{
	if (m_pUIObject == nullptr && m_pPopUpUIObject.size() == 0)
	{
		return;
	}

	std::vector<GameObject*>::iterator targetIter = m_pPopUpUIObject.end();
	std::vector<GameObject*>::iterator iter = m_pPopUpUIObject.begin();

	/// todo : ���� �Ѵ� ���� ���� �ذ� �� ��

	for (; iter != m_pPopUpUIObject.end(); ++iter)
	{
		// popui�� Ȱ��ȭ �� ���� �ִٸ�
		if ((*iter)->GetPopUpState())
		{
			targetIter = iter;

			m_pCurGameObject = *iter;

			SetPopUpUIVector(m_pCurGameObject);

			// ���� Ȱ��ȭ�� �� �Ǿ�������
			if (!m_bCurPopUpState)
			{
				m_curX = 0;
				m_curY = 0;

				if (m_pFocusedUI != nullptr)
				{
					if (m_pFocusedUI->GetChildrenComponent().size() > 1)
						m_pFocusedUI->FocusOut(m_pFocusedUI->GetChildrenComponent());
					else
						m_pFocusedUI->FocusOut();
				}

				m_pFocusedUI = dynamic_cast<UI*>(m_pCurGameObject->GetRootComponent()->GetChildrenComponent()[2]);
				if (m_pFocusedUI->GetChildrenComponent().size() > 1)
					m_pFocusedUI->FocusOn(m_pFocusedUI->GetChildrenComponent());
				else
					m_pFocusedUI->FocusOn(D2D1::ColorF::Yellow);
			}
			m_bCurPopUpState = true;
			break;
		}
	}
	// Ȱ��ȭ�� �˾��� ���� �� �⺻ UI�� ��Ŀ�� �ٲ��ֱ�
	if (targetIter == m_pPopUpUIObject.end())
	{
		m_pCurGameObject = m_pUIObject;

		std::vector<std::vector<UI*>>& uiLocation = m_UIObjectLocation;
		for (auto& curUIList : uiLocation)
		{
			// ó���� Focus ������ UI�� focus���ֱ�
			for (auto& focusUI : curUIList)
			{
				if (m_pFocusedUI != nullptr)
				{
					break;
				}
				if (focusUI != nullptr)
				{
					m_pFocusedUI = focusUI;
					if (m_pFocusedUI->GetChildrenComponent().size() > 1)
						m_pFocusedUI->FocusOn(m_pFocusedUI->GetChildrenComponent());
					else
						m_pFocusedUI->FocusOn(D2D1::ColorF::Yellow);
					break;
				}
			}
		}
		m_bCurPopUpState = false;
	}

	if (InputManager::GetInstance()->IsPadButtonPush(0, GamePadButtonCode::A))
	{
		if (m_pFocusedUI != nullptr)
			EventManager::GetInstance()->SendEvent(static_cast<ButtonUI*>(m_pFocusedUI)->GetButtonEventType());
	}
	// X������ �Ͻ�����(��������) �˾� ��(���� ������ �ٸ� ����� ���� ������ ������ ���)
	// startbox�� ���� ���� ���ߴ°Ŷ� �̰ŵ� ���ľ���
	// todo(ä��) : startbox�� �ٲ����. ���࿡ pause�� ���¸�  �̰� sendevent �ϱ�
	else if (InputManager::GetInstance()->IsPadButtonPush(0, GamePadButtonCode::START))
	{
		EventManager::GetInstance()->SendEvent(eEventType::CheckQuitGame);
	}

	/// todo(ä��) :�ѹ� �� ��ư�� ������ �� �� �ɸ��� �ؾ���

	if (m_pCurGameObject != nullptr)
	{
		
		m_curControllerPos = InputManager::GetInstance()->GetPadAxisLeftThumb(0);

		if (m_finalControllerPos != m_curControllerPos)
		{
			m_timeElapsed += deltaTime;
		}

		if (m_timeElapsed >= 0.2f)
		{
			m_timeElapsed = 0.f;
			m_bChangeFocusState = false;
		}
		else
		{
			if (!m_bChangeFocusState)
			{
				if (m_pFocusedUI != nullptr) // ��Ʈ�ѷ� ����
				{
					if (m_finalControllerPos == m_curControllerPos)
					{
						m_bChangeFocusState = false;
						return;
					}
					m_pFocusedUI = GetFocusedUI();
				}
				m_bChangeFocusState = true;
			}
		}
	}

	return;

	// ���콺
	// ���� ��Ŀ���� UI Ȯ��
	m_pFocusedUI = GetFocusedUI();

	// ��Ŀ���� UI�� ������ �ƹ��͵� �� �ϱ�
	if (!m_pFocusedUI)
	{
		return;
	}

	bool isLbtnClicked = InputManager::GetInstance()->IsKeyPush(eKeyCode::LBUTTON);
	bool isLbtnEnd = InputManager::GetInstance()->IsKeyEnd(eKeyCode::LBUTTON);

	if (m_pFocusedUI != nullptr)
	{
		if (isLbtnClicked)
		{
			m_pFocusedUI->MouseLbtnDown();
			m_pFocusedUI->SetLbtnDown(true);

			// printf("����\n");
		}
		else if (isLbtnEnd)
		{
			m_pFocusedUI->MouseLbtnUp();

			if (m_pFocusedUI->IsLbtnDown())
			{
				m_pFocusedUI->MouseLbtnClicked();
			}

			m_pFocusedUI->SetLbtnDown(false);
			// printf("����\n");
		}
	}
}


UI* UIManager::GetFocusedUI()
{
	// ���콺
	/*std::vector<Component*>& m_pChildUI = m_pCurUI->GetChildrenComponent();

	UI* pFocusedUI = m_pFocusedUI;

	std::vector<Component*>::iterator targetIter = m_pChildUI.end();
	std::vector<Component*>::iterator iter = m_pChildUI.begin();

	for (; iter != m_pChildUI.end(); ++iter)
	{
		if(static_cast<UI*>(*iter)->IsMouseOn())
		{
			targetIter = iter;
			pFocusedUI = static_cast<UI*>(*targetIter);
			pFocusedUI->FocusOn(D2D1::ColorF::Yellow);
			break;
		}
		else if (!(static_cast<UI*>(*iter)->IsMouseOn()) && m_pFocusedUI != nullptr)
		{
			m_pFocusedUI->FocusOut();
		}
	}

	if(targetIter == m_pChildUI.end())
	{
		return nullptr;
	}

	m_pChildUI.erase(targetIter);
	m_pChildUI.push_back(pFocusedUI);*/

	// ��Ʈ�ѷ�
	UI* pFocusedUI = m_pFocusedUI;

	/*float focusedY = m_pFocusedUI->GetFinalPos().y;
	float focusedX = m_pFocusedUI->GetFinalPos().x;*/

	/// todo : �˾� �����̰� ������ �� ����

	std::vector<std::vector<UI*>>& uiLocation = m_PopUpUIObjectLocation;

	if(m_bCurPopUpState)
	{
		uiLocation = m_PopUpUIObjectLocation;
	}
	else
	{
		uiLocation = m_UIObjectLocation;
	}

	// �Ʒ����� ���� ����ż� �����س�����
	const size_t storageX = m_curX;
	const size_t storageY = m_curY;

	// y�� ��ȭ���� x�� ��ȭ������ ���� ��
	if (abs(m_curControllerPos.x) < abs(m_curControllerPos.y))
	{
		// ����
		if (m_curControllerPos.y > m_finalControllerPos.y)
		{
			if (m_curY == 0)
			{
				return m_pFocusedUI;
			}
			m_curY -= 1;
		}

		// �Ʒ���
		else if (m_curControllerPos.y < m_finalControllerPos.y)
		{
			if (m_curY == uiLocation.size() - 1)
			{
				return m_pFocusedUI;
			}
			m_curY += 1;
		}

		//// �� �Ʒ��� �����̱�
		if (uiLocation[m_curY][m_curX] == nullptr)
		{
			size_t i = 0;
			while (i < uiLocation[m_curY].size())
			{
				if (i == m_pFocusedUI->GetXIndex())
				{
					i++;
					continue;
				}

				if (uiLocation[m_curY][i] != nullptr)
				{
					m_curX = i;
					pFocusedUI = uiLocation[m_curY][m_curX];
					break;
				}
				i++;
			}
		}
	}

	// x�� ��ȭ���� y�� ��ȭ������ ���� ��
	else
	{
		// ��������
		if (m_curControllerPos.x < m_finalControllerPos.x)
		{
			if (m_curX == 0)
			{
				return m_pFocusedUI;
			}
			m_curX -= 1;
		}

		// ����������
		else if (m_curControllerPos.x > m_finalControllerPos.x)
		{
			if (m_curX == uiLocation[0].size() - 1)
			{
				return m_pFocusedUI;
			}
			m_curX += 1;
		}

		//// �翷���� �����̱�
		if (uiLocation[m_curY][m_curX] == nullptr)
		{
			size_t i = 0;
			while (i < uiLocation.size())
			{
				if (m_curX == uiLocation[m_curY].size())
				{
					break;
				}
				if (uiLocation[i][m_curX] != nullptr)
				{
					m_curY = i;
					pFocusedUI = uiLocation[m_curY][m_curX];
					break;
				}
				i++;
			}
		}
	}

	if (uiLocation[m_curY][m_curX] != nullptr)
	{
		pFocusedUI = uiLocation[m_curY][m_curX];
		if (m_pFocusedUI->GetChildrenComponent().size() > 1)
			m_pFocusedUI->FocusOut(m_pFocusedUI->GetChildrenComponent());
		else
			m_pFocusedUI->FocusOut();
		m_curControllerPos = { 0.f, 0.f };

		if (pFocusedUI->GetChildrenComponent().size() > 1)
			pFocusedUI->FocusOn(pFocusedUI->GetChildrenComponent());
		else
			pFocusedUI->FocusOn(D2D1::ColorF::Yellow);
		return pFocusedUI;
	}
	else
	{
		m_curX = storageX;
		m_curY = storageY;
		return m_pFocusedUI;
	}
}

void UIManager::ClearUIObject()
{
	m_curX = 0;
	m_curY = 0;
	m_pFocusedUI = nullptr;
	m_pCurGameObject = nullptr;
	m_bCurPopUpState = false;
	m_bChangeFocusState = false;
	m_pUIObject = nullptr;
	m_UIObjectLocation.clear();
	m_PopUpUIObjectLocation.clear();
	m_pPopUpUIObject.clear();
	m_timeElapsed = 0.f;
}

// InputManager���� ���������� ���� ��ǲ�� ���� ����

