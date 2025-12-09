#pragma once

#include "../ecs/ComponentManager.h"
#include "../ecs/System.h"

class CollisionSystem : public System {
public:
    CollisionSystem(ComponentManager& componentManager) : componentManager(componentManager) {}

    void update(float deltaTime) override;

    static bool checkCollision(const float x1, const float y1, const float w1, const float h1,
                               const float x2, const float y2, const float w2, const float h2) {
        return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
    }

private:
    ComponentManager& componentManager;
};