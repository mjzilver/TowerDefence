#pragma once

#include <glm/glm.hpp>
#include "../texture/TextureManager.h"
#include "Tile.h"

class TileFactory {
public:
    static Tile createGrassTile(glm::vec2 position, TextureManager& textureManager);
};
