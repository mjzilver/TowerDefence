#pragma once

#include "../ecs/System.h"

class MovementSystem : public System {
public:
    MovementSystem(EngineContext& ctx) : System(ctx) {}

    void update(float deltaTime) override;

    void reset() override {};
};