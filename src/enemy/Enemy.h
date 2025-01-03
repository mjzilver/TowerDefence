#pragma once

#include <glm/glm.hpp>
#include "../texture/Texture.h"
#include "../utils/Direction.h"
#include "../tile/Tile.h"	

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

    // cast to tile 
    // TODO: fix using ECS
    Tile toTile() const {
        Tile tile;
        tile.position = position;
        tile.size = size;
        tile.texture = texture;
        tile.textureCoords = textureCoords;
        return tile;
    }
};
