#pragma once

#include "../ecs/Component.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

struct PathfindingComponent : public Component {
    float x;
    float y;

    int currentIndex = 0;

    glm::vec2 randomOffset = {0.0f, 0.0f};
};