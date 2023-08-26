#pragma once

#include "../Engine/GameObject.h"

#include <vector>

#include "Train.h"

class DebugRenderComponent;
class TrainPattern;
class TrafficObject;

enum class TRAIN_TYPE;
enum class TRAIN_DROP_TYPE;

class TrainSpawner :
    public GameObject
{
private:
    TrafficObject* m_pTrafficObject = nullptr;
    DebugRenderComponent* m_pDebugRenderComponent = nullptr;

public:
    float m_ElapsedTriggerTime = 0.f;
    float m_TriggerTime = 5.f;
    float m_ElapsedDelayTime = 0.f;
    float m_DelayTime = 1.f;
    float m_ElapsedSpawnTime = 0.f;
    float m_SpawnTime = 5.f;
    std::vector<TrainPattern*> m_TrainPatterns;
    std::vector<framework::Vector2D> m_SpawnPositions;
    static TRAIN_TYPE m_TrainTypeArr[static_cast<UINT>(TRAIN_TYPE::END)];
    static TRAIN_DROP_TYPE m_DropTypeArr[static_cast<UINT>(TRAIN_DROP_TYPE::END)];

private:
    bool m_isRunning = false;

public:
    TrainSpawner();
    ~TrainSpawner() final;

public:
    bool Initialize() final;
    void Update(float deltaTime) final;

public:
    void LinkTrafficObject(TrafficObject* trafficObj) { m_pTrafficObject = trafficObj; }
    void SetRunning(const bool isRunning) { m_isRunning = isRunning; }

    float GetTriggerTime() const { return m_TriggerTime; }
    void SetTriggerTime(const float spawnTime) { m_TriggerTime = spawnTime; }

    float GetSpawnTime() const { return m_SpawnTime; }
    void SetSpawnTime(const float spawnTime) { m_SpawnTime = spawnTime; }

    bool GetIsRunning() const { return m_isRunning; }
    void SetIsRunning(const bool isRunning) { m_isRunning = isRunning; }

public:
    void AddSpawnPosition(const framework::Vector2D& spawnPosition) { m_SpawnPositions.push_back(spawnPosition); }
};