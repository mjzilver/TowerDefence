#include "EnemyFactory.h"
#include "../utils/TextureCoords.h"

// TODO
const int textureWidth = 128;
const int textureHeight = 64;

Enemy EnemyFactory::createEnemey(glm::vec2 position, TextureManager& textureManager, const std::string& textureName, int textureX, int textureY) {
    Enemy enemy;
    enemy.position = position;
    enemy.size = glm::vec2(Enemy::TILE_WIDTH, Enemy::TILE_HEIGHT);
    enemy.texture = textureManager.loadTexture(textureName);

    enemy.textureCoords = getTextureCoords(textureX, textureY, textureWidth, textureHeight, enemy.texture.size.x, enemy.texture.size.y);
    return enemy;
}

Enemy EnemyFactory::createFireBug(glm::vec2 position, TextureManager& textureManager) {
    Enemy en = createEnemey(position, textureManager, "enemy/Firebug.png", 0, 0);
    en.frameCount = 8;
    en.frameSpeed = 100; // ticks
    en.baseTextureCoords = glm::vec2(0, 0);

    en.direction = Direction::West;

    en.state = State::Walking;

    return en;
}