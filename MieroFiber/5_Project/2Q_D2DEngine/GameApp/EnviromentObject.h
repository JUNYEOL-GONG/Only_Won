#pragma once

#include "../Engine/GameObject.h"

class TextureComponent;
class BoxCollider2D;

/// <summary>
/// 맵의 배치물 / 장애물 오브젝트 (콜라이더 O)
/// </summary>
class EnviromentObject :
    public GameObject
{
private:
    TextureComponent* m_pTextureComponent = nullptr;
    BoxCollider2D* m_pBoxCollider2D = nullptr;

public:
    EnviromentObject();
    ~EnviromentObject() final;

public:
    bool Initialize() final;
    void Update(float deltaTime) final;

public:
    void OnCollisionEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter) final;
    void OnCollisionStay(Collider2D* otherCollision) final;
    void OnCollisionExit(Collider2D* otherCollision) final;

    void OnTriggerEnter(Collider2D* thisCollision, Collider2D* otherCollision, std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter) final;
    void OnTriggerStay(Collider2D* otherCollision) final;
    void OnTriggerExit(Collider2D* otherCollision) final;

public:
    void SetTexture(const WCHAR* szFilePath, const std::wstring textureName) const;
    void SetObstaclePos(float x, float y) const;
    void SetObstacleRotation(float angle) const;
    void SetColliderPos(float x, float y) const;
    void SetColliderExtend(float x, float y) const;

};