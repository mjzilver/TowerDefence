#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "../ecs/Component.h"

struct PathfindingComponent : public Component {
    float x;
    float y;

    int currentIndex = 0;

    bool reachedGoal = false;

    glm::vec2 randomOffset = {0.0f, 0.0f};
};