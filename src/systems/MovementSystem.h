#pragma once
#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"
#include "../components/PositionComponent.h"
#include "../components/VelocityComponent.h"

class MovementSystem : public System {
public:
    MovementSystem(ComponentManager& componentManager) 
        : componentManager(componentManager) {}

    void update(float deltaTime) override {
        for (Entity entity : entities) {
            auto* position = componentManager.getComponent<PositionComponent>(entity);
            auto* velocity = componentManager.getComponent<VelocityComponent>(entity);

            if (position && velocity) {
                position->x += velocity->x * deltaTime;
                position->y += velocity->y * deltaTime;
            }
        }
    }

private:
    ComponentManager& componentManager;
};