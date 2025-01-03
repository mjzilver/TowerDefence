#pragma once

#include <glm/glm.hpp>

#include "../texture/Texture.h"
#include "../tile/Tile.h"
#include "../utils/Direction.h"
#include "../utils/TextureCoords.h"

enum EnemyType {
    FireBug,
    LeafBug,
    MagmaCrab,
    Scorpion,
};

// Note: to get west just flip the texture horizontally
enum AnimationRow {
    IdleFacingSouth = 0,
    IdleFacingNorth = 1,
    IdleFacingEast = 2,
    WalkingFacingSouth = 3,
    WalkingFacingNorth = 4,
    WalkingFacingEast = 5,
    AttackingFacingSouth = 6,
    AttackingFacingNorth = 7,
    AttackingFacingEast = 8,
};

enum State {
    Idle,
    Walking,
    Attacking,
};

// framecount per state
const int IDLE_FRAME_COUNT = 6;
const int WALKING_FRAME_COUNT = 8;
const int ATTACKING_FRAME_COUNT = 11;

struct Enemy {
    // Keep aspect ratio of 2:1
    static const int TILE_WIDTH = 120;
    static const int TILE_HEIGHT = 60;

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
    glm::vec2 baseTextureCoords;
    State state;

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
        tile.direction = direction;
        return tile;
    }

    const int textureWidth = 128;
    const int textureHeight = 64;

    void updateTextureCoords() {
        AnimationRow animationRow;

        int directionInt = static_cast<int>(direction);
        if (direction == Direction::West) {
            directionInt = static_cast<int>(Direction::East);
        }

        // Determine the animation state
        switch (state) {
            case Idle:
                animationRow = static_cast<AnimationRow>(IdleFacingSouth + directionInt);
                frameCount = IDLE_FRAME_COUNT;
                break;
            case Walking:
                animationRow = static_cast<AnimationRow>(WalkingFacingSouth + directionInt);
                frameCount = WALKING_FRAME_COUNT;
                break;
            case Attacking:
                animationRow = static_cast<AnimationRow>(AttackingFacingSouth + directionInt);
                frameCount = ATTACKING_FRAME_COUNT;
                break;
        }

        textureCoords =
            getTextureCoords(frame + baseTextureCoords.x, baseTextureCoords.y + static_cast<int>(animationRow),
                             textureWidth, textureHeight, texture.size.x, texture.size.y);
    }
};
