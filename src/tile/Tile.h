#pragma once

#include <glm/glm.hpp>
#include "../texture/Texture.h"
#include "TileType.h"

struct Tile {
    static const int TILE_SIZE = 80;

    glm::vec2 position;
    glm::vec2 size;
    Texture texture;
    glm::vec4 textureCoords;
};

// positionComponent, sizeComponent, textureComponent