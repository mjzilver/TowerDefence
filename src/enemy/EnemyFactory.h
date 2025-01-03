#pragma once

#include <glm/glm.hpp>
#include "../texture/TextureManager.h"
#include "Enemy.h"

class EnemyFactory {
public:
    static Enemy createFireBug(glm::vec2 position, TextureManager& textureManager);

private:
    static Enemy createEnemey(glm::vec2 position, TextureManager& textureManager, const std::string& textureName, int textureX, int textureY);	
};