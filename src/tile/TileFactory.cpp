#include "TileFactory.h"

// TODO
const int textureSize = 64;
Tile TileFactory::createTile(glm::vec2 position, TextureManager& textureManager, const std::string& textureName, int textureX, int textureY) {
    Tile tile;
    tile.position = position;
    tile.size = glm::vec2(Tile::TILE_SIZE, Tile::TILE_SIZE);
    tile.texture = textureManager.loadTexture(textureName);

    int textureTopY = tile.texture.size.y - textureSize;

    tile.textureCoords = glm::vec4(textureX * textureSize, textureTopY - (textureSize * textureY), textureSize, textureSize);

    return tile;
}

Tile TileFactory::createGrassTile(glm::vec2 position, TextureManager& textureManager) {
    return createTile(position, textureManager, "Grass.png", 2, 1);
}

Tile TileFactory::createPathTile(glm::vec2 position, TextureManager& textureManager) {
    return createTile(position, textureManager, "Grass.png", 9, 2);
}

// TODO 
Tile TileFactory::createStartTile(glm::vec2 position, TextureManager& textureManager) { 
    return createPathTile(position, textureManager);
}

// TODO
Tile TileFactory::createEndTile(glm::vec2 position, TextureManager& textureManager) {
    return createPathTile(position, textureManager);
}
