#pragma once

#include <glm/glm.hpp>

#include "../ecs/Component.h"
#include "../texture/Texture.h"

struct TextureComponent : public Component {
    Texture texture;
    glm::vec4 coords;
    bool flipped = false;
};