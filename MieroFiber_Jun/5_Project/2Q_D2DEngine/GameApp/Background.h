#pragma once

#include "../Engine/GameObject.h"

class TextureComponent;

/// <summary>
/// 맵의 배경 오브젝트 (콜라이더 X)
/// </summary>
class Background :
    public GameObject
{
private:
    TextureComponent* m_pFloor = nullptr;
    TextureComponent* m_pPebbles = nullptr;

    TextureComponent* m_pLeftCliff = nullptr;
    TextureComponent* m_pRightCliff = nullptr;

    TextureComponent* m_pTrack_1 = nullptr;
    TextureComponent* m_pTrack_2 = nullptr;
    TextureComponent* m_pTrack_3 = nullptr;

    TextureComponent* m_pHammer = nullptr;
    TextureComponent* m_pPickaxe = nullptr;
    TextureComponent* m_pShovel = nullptr;
    TextureComponent* m_pSkull = nullptr;
    TextureComponent* m_pTruckMoney = nullptr;

public:
    Background();
    ~Background() final;

public:
    bool Initialize() final;
    void Update(float deltaTime) final;
};