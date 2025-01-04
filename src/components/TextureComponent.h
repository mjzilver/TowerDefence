#pragma once

#include "../ecs/Component.h"
#include "../texture/Texture.h"

#include <glm/glm.hpp>

struct TextureComponent : public Component {
    Texture texture;
    glm::vec4 coords;
    int zIndex = 1;
    bool flipped = false;
};