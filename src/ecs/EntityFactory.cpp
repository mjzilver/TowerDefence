#include "EntityFactory.h"

#include "../components/PositionComponent.h"
#include "../components/TextureComponent.h"
#include "../components/SizeComponent.h"
#include "../components/DirectionComponent.h"
#include "../components/FlagComponent.h"
#include "../components/AnimationComponent.h"

#include "../utils/TextureCoords.h"
#include "../utils/Direction.h"	
#include "../utils/State.h"

static const int TILE_SIZE = 80;

Entity EntityFactory::createGrassTile(glm::vec2 position, TextureManager& textureManager) {
    Entity entity = entityManager.createEntity();

    static const int TEXTURE_SIZE = 64;

    PositionComponent positionComponent;
    positionComponent.x = position.x;
    positionComponent.y = position.y;
    componentManager.addComponent(entity, positionComponent);

    SizeComponent sizeComponent;
    sizeComponent.w = TILE_SIZE;
    sizeComponent.h = TILE_SIZE;
    componentManager.addComponent(entity, sizeComponent);

    TextureComponent textureComponent;
    textureComponent.texture = textureManager.loadTexture("tiles/Grass.png");
    textureComponent.coords = getTextureCoords(2, 1, TEXTURE_SIZE, TEXTURE_SIZE, textureComponent.texture.size.x, textureComponent.texture.size.y);
    componentManager.addComponent(entity, textureComponent);

    return entity;
}

Entity EntityFactory::createPathTile(glm::vec2 position, TextureManager& textureManager) { 
    Entity entity = entityManager.createEntity();

    static const int TEXTURE_SIZE = 64;

    PositionComponent positionComponent;
    positionComponent.x = position.x;
    positionComponent.y = position.y;
    componentManager.addComponent(entity, positionComponent);

    SizeComponent sizeComponent;
    sizeComponent.w = TILE_SIZE;
    sizeComponent.h = TILE_SIZE;
    componentManager.addComponent(entity, sizeComponent);

    TextureComponent textureComponent;
    textureComponent.texture = textureManager.loadTexture("tiles/Grass.png");
    textureComponent.coords = getTextureCoords(9, 2, TEXTURE_SIZE, TEXTURE_SIZE, textureComponent.texture.size.x, textureComponent.texture.size.y);
    componentManager.addComponent(entity, textureComponent);

    return entity;
}

Entity EntityFactory::createFireBug(glm::vec2 position, TextureManager& textureManager) {
    Entity entity = entityManager.createEntity();

    static const int TEXTURE_WIDTH = 128;
    static const int TEXTURE_HEIGHT = 64;
    static const int ENEMY_WIDTH = 128;
    static const int ENEMY_HEIGHT = 64;

    PositionComponent positionComponent;
    positionComponent.x = position.x;
    positionComponent.y = position.y;
    componentManager.addComponent(entity, positionComponent);

    SizeComponent sizeComponent;
    sizeComponent.w = ENEMY_WIDTH;
    sizeComponent.h = ENEMY_HEIGHT;
    componentManager.addComponent(entity, sizeComponent);

    TextureComponent textureComponent;
    textureComponent.texture = textureManager.loadTexture("enemy/Firebug.png");
    textureComponent.coords = getTextureCoords(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT, textureComponent.texture.size.x, textureComponent.texture.size.y);
    textureComponent.zIndex = 2; // render on top of the path
    componentManager.addComponent(entity, textureComponent);

    DirectionComponent directionComponent;
    directionComponent.direction = Direction::South;
    componentManager.addComponent(entity, directionComponent);

    /* 
    Rows in the texture atlas:
    IdleFacingSouth = 0,
    IdleFacingNorth = 1,
    IdleFacingEast = 2,
    WalkingFacingSouth = 3,
    WalkingFacingNorth = 4,
    WalkingFacingEast = 5,
    AttackingFacingSouth = 6,
    AttackingFacingNorth = 7,
    AttackingFacingEast = 8,
    */

    AnimationComponent animationComponent;
    animationComponent.frame = 0;
    animationComponent.time = 0;
    animationComponent.frameSize = glm::vec2(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    animationComponent.frameCount = std::map<State, int>{
        {State::Idle, 6},
        {State::Walking, 8},
        {State::Attacking, 11},
    };
    animationComponent.frameDuration = 0.5f; // seconds
    animationComponent.baseTextureCoords = glm::vec2(0, 0);
    animationComponent.state = State::Walking;
    componentManager.addComponent(entity, animationComponent);

    // Add hitbox (about 50% of the size)
    
    return entity;
}
