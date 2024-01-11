#pragma once

#include "../Engine/GameObject.h"

enum class TRAIN_DROP_TYPE
{
    NONE,
    MONEY,
    ITEM,

    END
};

enum class TRAIN_TYPE
{
    ABC,
    ABB,
    ABD,
    ABCB,
    ABBBB,

    END
};

class TextureComponent;
class BoxComponent;
class BoxCollider2D;
class TrainSpawner;
class TrainPattern;

class Train :
    public GameObject
{
private:
    TextureComponent* m_pTextureComponent = nullptr;
    BoxCollider2D* m_pBoxCollider2D = nullptr;
    BoxCollider2D* m_pHeadBoxCollider = nullptr;

private:
    TrainSpawner* m_pSpawner = nullptr;
    TrainPattern* m_pPattern = nullptr;
    TRAIN_TYPE m_TrainType = TRAIN_TYPE::ABBBB;
    TRAIN_DROP_TYPE m_DropType = TRAIN_DROP_TYPE::NONE;
    bool m_IsStop = false;
    float m_Speed = 500.f;
    int m_Direction = 1;
    framework::Vector2D m_SpawnPosition = { 0.f, 0.f };
    framework::Vector2D m_Velocity = { 0.f, 0.f };
public:
    Train();
    ~Train() final;

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
	void SetSpawner(TrainSpawner* pSpawner) { m_pSpawner = pSpawner; }
    TrainSpawner* GetSpawner() const { return m_pSpawner; }

    void SetPattern(TrainPattern* pPattern) { m_pPattern = pPattern; }
    TrainPattern* GetPattern() const { return m_pPattern; }

    TRAIN_TYPE GetTrainType() const { return m_TrainType; }
    void SetTrainType(const TRAIN_TYPE trainType) { m_TrainType = trainType; }

    TRAIN_DROP_TYPE GetDropType() const { return m_DropType; }
    void SetDropType(const TRAIN_DROP_TYPE dropType) { m_DropType = dropType; }

	float GetSpeed() const { return m_Speed; }
	void SetSpeed(const float speed) { m_Speed = speed; }

    int GetDirection() const { return m_Direction; }
    void SetDirection(const int direction) { m_Direction = direction; }

    framework::Vector2D GetSpawnPosition() const { return m_SpawnPosition; }
    void SetSpawnPosition(const framework::Vector2D& spawnPosition) { m_SpawnPosition = spawnPosition; }
    void SetSpawnPosition(float x, float y) { m_SpawnPosition = { x,y }; }
};