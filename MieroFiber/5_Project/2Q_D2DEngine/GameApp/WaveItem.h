#pragma once
#include "ItemObject.h"

class AnimationComponent;

class WaveItem :
    public ItemObject
{
private:
    AnimationComponent* m_pEffectAnimation = nullptr;

public:
    bool Initialize() final;
    void Update(const float deltaTime) final;
};

