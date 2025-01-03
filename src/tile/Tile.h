#pragma once

#include <glm/glm.hpp>
#include "../texture/Texture.h"

struct Tile {
    glm::vec2 position;
    glm::vec2 size;
    Texture texture;
    glm::vec4 textureCoords;
};


