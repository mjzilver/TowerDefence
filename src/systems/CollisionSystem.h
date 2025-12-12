#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
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

    static bool contains(const glm::vec4& rect, const glm::vec2& point) {
        return point.x >= rect.x && point.x <= rect.x + rect.z &&
               point.y >= rect.y && point.y <= rect.y + rect.w;
    }

    static bool contains(const float x1, const float y1, const float w1, const float h1,
                               const float x2, const float y2) {
        return x1 < x2 && x1 + w1 > x2 && y1 < y2 && y1 + h1 > y2;
    }

    void reset() override { return; };  

private:
    ComponentManager& componentManager;
};