#pragma once

#include "../ecs/System.h"

class ShootingSystem : public System {
public:
    ShootingSystem(EngineContext& ctx);

    void update(float deltaTime) override;

    void reset() override {};
};