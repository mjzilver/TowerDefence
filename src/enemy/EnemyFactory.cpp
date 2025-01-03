#include "EnemyFactory.h"

// TODO
// North-South facing (for West-East the height is halved)
const int textureWidth = 128;
const int textureHeight = 64;

Enemy EnemyFactory::createEnemey(glm::vec2 position, TextureManager& textureManager, const std::string& textureName, int textureX, int textureY) {
    Enemy enemy;
    enemy.position = position;
    enemy.size = glm::vec2(Enemy::TILE_SIZE, Enemy::TILE_SIZE);
    enemy.texture = textureManager.loadTexture(textureName);

    int textureTopY = enemy.texture.size.y - textureHeight;

    enemy.textureCoords = glm::vec4(textureX * textureWidth, textureTopY - (textureHeight * textureY), textureWidth, textureHeight);
    return enemy;
}

Enemy EnemyFactory::createFireBug(glm::vec2 position, TextureManager& textureManager) {
    Enemy en = createEnemey(position, textureManager, "enemy/Firebug.png", 0, 0);
    en.frameCount = 6;
    en.frameSpeed = 100; // ms

    return en;
}