#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../ecs/Component.h"

struct TextComponent : public Component {
    std::string text;
    glm::vec3 color;
};