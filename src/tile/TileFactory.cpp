#include "TileFactory.h"

// TODO
const int tileSize = 80;
const int textureSize = 64;

Tile TileFactory::createGrassTile(glm::vec2 position, TextureManager& textureManager) {
    Tile tile;
    tile.position = position;
    tile.size = glm::vec2(tileSize, tileSize);
    tile.texture = textureManager.loadTexture("Grass.png");

    int textureTopY = tile.texture.size.y - textureSize;
    int textureX = 2;
    int textureY = 1;

    tile.textureCoords = glm::vec4(textureX * textureSize, textureTopY - (textureSize * textureY), textureSize, textureSize);

    return tile;
}
