#pragma once

#include "../ecs/ComponentManager.h"
#include "../ecs/System.h"

class MovementSystem : public System {
public:
    MovementSystem(ComponentManager& componentManager) : componentManager(componentManager) {}

    void update(float deltaTime) override;

private:
    ComponentManager& componentManager;
};