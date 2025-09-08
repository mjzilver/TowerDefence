#pragma once

#include "../ecs/ComponentManager.h"
#include "../ecs/System.h"

class CollisionSystem : public System {
public:
    CollisionSystem(ComponentManager& componentManager) : componentManager(componentManager) {}

    void update(float deltaTime) override;

    static bool checkCollision(const float X1, const float Y1, const float W1, const float H1,
                               const float X2, const float Y2, const float W2, const float H2) {
        return X1 < X2 + W2 && X1 + W1 > X2 && Y1 < Y2 + H2 && Y1 + H1 > Y2;
    }

private:
    ComponentManager& componentManager;
};