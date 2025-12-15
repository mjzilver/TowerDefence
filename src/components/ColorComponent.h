#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "../ecs/Component.h"

struct ColorComponent : public Component {
    glm::vec3 color;
};