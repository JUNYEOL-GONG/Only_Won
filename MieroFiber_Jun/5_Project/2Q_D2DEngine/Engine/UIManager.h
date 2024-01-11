#pragma once
#include <list>
#include <vector>
#include "struct.h"

class PopUpUIObject;
class World;
class Component;
class UI;
class UIObject;
class SceneComponent;
class GameObject;

class UIManager
{
public:
	~UIManager();

public:
	void SetWorldUIObject(std::vector<GameObject*>& curObject);
	void SetPopUpUIVector(GameObject* curObject);
	void SetWorldPopUpUIObject(std::vector<GameObject*>& curObject);
	void Update(float deltaTime);

public:
	UI* GetFocusedUI() ;
	void ClearUIObject();

private:
	UI* m_pFocusedUI = nullptr;

	float m_timeElapsed = 0.f;

	GameObject* m_pUIObject;
	std::vector<GameObject*> m_pPopUpUIObject;

	// 현재 게임오브젝트(걍 UI인지 PopUI인지)
	GameObject* m_pCurGameObject = nullptr;

	bool m_bMenuClicked = false;
	bool m_bColorChanged = false;

	// 1초동안 못 움직이게 하기
	bool m_bChangeFocusState = false;

	// ㅠㅠ 어쩔 수 없이 또 필요하다..
	bool m_bCurPopUpState = false;

	D2D1::ColorF m_color = D2D1::ColorF::Black;

	framework::Vector2D m_finalControllerPos = { 0.f, 0.f };
	framework::Vector2D m_curControllerPos = { 0.f, 0.f };

	size_t m_curX = 0;
	size_t m_curY = 0;

	std::vector<std::vector<UI*>> m_UIObjectLocation;
	std::vector<std::vector<UI*>> m_PopUpUIObjectLocation;
};