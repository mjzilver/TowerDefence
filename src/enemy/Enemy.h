#pragma once

#include <glm/glm.hpp>
#include "../texture/Texture.h"
#include "../utils/Direction.h"

enum EnemyType {
    FireBug,
    LeafBug,
    MagmaCrab,
    Scorpion,
};

struct Enemy {
    static const int TILE_SIZE = 80;

    // PositionComponent
    glm::vec2 position;
    glm::vec2 size;

    // Velocity Component
    glm::vec2 velocity;

    // Cardinal Direction Component
    Direction direction;

    // Health Component
    int health;

    // Enemy Type Component
    EnemyType type;

    // Animation Component
    int frame;
    int frameCount;
    int frameSpeed;

    // Texture Component
    Texture texture;
    glm::vec4 textureCoords;
};
