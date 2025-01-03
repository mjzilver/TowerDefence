#pragma once

#include <glm/glm.hpp>
#include "../texture/TextureManager.h"
#include "Tile.h"
#include "TileType.h"

class TileFactory {
public:
    static Tile createGrassTile(glm::vec2 position, TextureManager& textureManager);
    static Tile createPathTile(glm::vec2 position, TextureManager& textureManager);
    static Tile createStartTile(glm::vec2 position, TextureManager& textureManager);
    static Tile createEndTile(glm::vec2 position, TextureManager& textureManager);
private:
    static Tile createTile(glm::vec2 position, TextureManager& textureManager, const std::string& textureName, int textureX, int textureY);	
};