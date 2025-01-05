#pragma once

#include "../ecs/Component.h"
#include "../texture/Texture.h"
#include "../utils/ZLayer.h"

#include <glm/glm.hpp>

struct TextureComponent : public Component {
    Texture texture;
    glm::vec4 coords;
    ZLayer zIndex = ZLayer::Ground;
    bool flipped = false;
};