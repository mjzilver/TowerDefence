#pragma once

#include "../ecs/System.h"

class ShootingSystem : public System {
public:
    ShootingSystem(EngineContext& ctx) : System(ctx) {}

    void update(float deltaTime) override;

    void reset() override {};
};