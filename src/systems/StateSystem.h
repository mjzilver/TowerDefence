#pragma once

#include "../ecs/ComponentManager.h"
#include "../ecs/System.h"

class StateSystem : public System {
public:
    StateSystem(ComponentManager& componentManager) : componentManager(componentManager) {}

    void update(float deltaTime) override;

private:
    ComponentManager& componentManager;
};