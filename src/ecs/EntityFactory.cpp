#include "EntityFactory.h"

#include "../components/AnimationComponent.h"
#include "../components/ChildComponent.h"
#include "../components/CollisionComponent.h"
#include "../components/DamageComponent.h"
#include "../components/DirectionComponent.h"
#include "../components/FlagComponent.h"
#include "../components/HealthComponent.h"
#include "../components/PathfindingComponent.h"
#include "../components/PositionComponent.h"
#include "../components/RotationComponent.h"
#include "../components/SizeComponent.h"
#include "../components/SpeedComponent.h"
#include "../components/TextureComponent.h"
#include "../components/VelocityComponent.h"
#include "../components/WeaponComponent.h"
#include "../utils/Direction.h"
#include "../utils/Globals.h"
#include "../utils/State.h"
#include "../utils/TextureCoords.h"
#include "../utils/ZLayer.h"

Entity EntityFactory::createGrassTile(glm::vec2 position) {
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

Entity EntityFactory::createPathTile(glm::vec2 position) {
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

    FlagComponent flagComponent;
    flagComponent.type = FlagType::Path;
    componentManager.addComponent(entity, flagComponent);

    TextureComponent textureComponent;
    textureComponent.texture = textureManager.loadTexture("tiles/Grass.png");
    textureComponent.coords = getTextureCoords(9, 2, TEXTURE_SIZE, TEXTURE_SIZE, textureComponent.texture.size.x, textureComponent.texture.size.y);
    componentManager.addComponent(entity, textureComponent);

    return entity;
}

Entity EntityFactory::createFireBug(glm::vec2 position) {
    Entity entity = entityManager.createEntity();

    static const int TEXTURE_WIDTH = 128;
    static const int TEXTURE_HEIGHT = 64;
    static const int ENEMY_WIDTH = 128;
    static const int ENEMY_HEIGHT = 64;

    PositionComponent positionComponent;
    positionComponent.x = position.x;
    positionComponent.y = position.y;
    componentManager.addComponent(entity, positionComponent);

    VelocityComponent velocityComponent;
    velocityComponent.x = 0;
    velocityComponent.y = 0;
    componentManager.addComponent(entity, velocityComponent);

    HealthComponent healthComponent;
    healthComponent.health = 100;
    componentManager.addComponent(entity, healthComponent);

    SpeedComponent speedComponent;
    speedComponent.speed = 100;
    componentManager.addComponent(entity, speedComponent);

    SizeComponent sizeComponent;
    sizeComponent.w = ENEMY_WIDTH;
    sizeComponent.h = ENEMY_HEIGHT;
    componentManager.addComponent(entity, sizeComponent);

    TextureComponent textureComponent;
    textureComponent.texture = textureManager.loadTexture("enemy/Firebug.png");
    textureComponent.coords = getTextureCoords(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT, textureComponent.texture.size.x, textureComponent.texture.size.y);
    textureComponent.zIndex = ZLayer::Enemy;
    componentManager.addComponent(entity, textureComponent);

    DirectionComponent directionComponent;
    directionComponent.direction = Direction::South;
    componentManager.addComponent(entity, directionComponent);

    AnimationComponent animationComponent;
    animationComponent.frame = 0;
    animationComponent.time = 0;
    animationComponent.frameSize = glm::vec2(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    animationComponent.frameCount = std::map<State, int>{
        {State::Idle, 6},
        {State::Active, 8},
        {State::Dead, 11},
    };
    animationComponent.stateDirectionRowMap = std::map<std::pair<State, Direction>, int>{
        {{State::Idle, Direction::South}, 0},  {{State::Idle, Direction::North}, 1},   {{State::Idle, Direction::East}, 2},
        {{State::Idle, Direction::West}, 2},   {{State::Active, Direction::South}, 3}, {{State::Active, Direction::North}, 4},
        {{State::Active, Direction::East}, 5}, {{State::Active, Direction::West}, 5},  {{State::Dead, Direction::South}, 6},
        {{State::Dead, Direction::North}, 7},  {{State::Dead, Direction::East}, 8},    {{State::Dead, Direction::West}, 8},
    };
    animationComponent.frameDuration = 0.2f;  // seconds
    animationComponent.baseTextureCoords = glm::vec2(0, 0);
    animationComponent.state = State::Active;
    componentManager.addComponent(entity, animationComponent);

    PathfindingComponent pathfindingComponent;
    pathfindingComponent.x = 0;
    pathfindingComponent.y = 0;
    componentManager.addComponent(entity, pathfindingComponent);

    const float HITBOX_SCALE = 0.5f;

    // hitbox is smaller than the texture
    CollisionComponent collisionComponent;
    collisionComponent.x = ENEMY_WIDTH * (1.0f - HITBOX_SCALE) / 2.0f;
    collisionComponent.y = ENEMY_HEIGHT * (1.0f - HITBOX_SCALE) / 2.0f;
    collisionComponent.w = ENEMY_WIDTH * HITBOX_SCALE;
    collisionComponent.h = ENEMY_HEIGHT * HITBOX_SCALE;
    componentManager.addComponent(entity, collisionComponent);

    return entity;
}

Entity EntityFactory::createTower(glm::vec2 position) {
    Entity entity = entityManager.createEntity();

    static const int TEXTURE_WIDTH = 64;
    static const int TEXTURE_HEIGHT = 128;
    static const int TOWER_WIDTH = 64;
    static const int TOWER_HEIGHT = 128;

    PositionComponent positionComponent;
    positionComponent.x = position.x;
    positionComponent.y = position.y;
    componentManager.addComponent(entity, positionComponent);

    SizeComponent sizeComponent;
    sizeComponent.w = TOWER_WIDTH;
    sizeComponent.h = TOWER_HEIGHT;
    componentManager.addComponent(entity, sizeComponent);

    TextureComponent textureComponent;
    textureComponent.texture = textureManager.loadTexture("towers/tower1/Tower 01.png");
    textureComponent.coords = getTextureCoords(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT, textureComponent.texture.size.x, textureComponent.texture.size.y);
    textureComponent.zIndex = ZLayer::Tower;
    componentManager.addComponent(entity, textureComponent);

    Entity weapon = entityManager.createEntity();

    const int WEAPON_SIZE = 96;

    PositionComponent weaponPositionComponent;
    weaponPositionComponent.x = position.x + (TOWER_WIDTH - WEAPON_SIZE) / 2;
    weaponPositionComponent.y = position.y;
    componentManager.addComponent(weapon, weaponPositionComponent);

    SizeComponent weaponSizeComponent;
    weaponSizeComponent.w = WEAPON_SIZE;
    weaponSizeComponent.h = WEAPON_SIZE;
    componentManager.addComponent(weapon, weaponSizeComponent);

    TextureComponent weaponTextureComponent;
    weaponTextureComponent.texture = textureManager.loadTexture("towers/tower1/Tower 01 - Level 01 - Weapon.png");
    weaponTextureComponent.coords =
        getTextureCoords(0, 0, WEAPON_SIZE, WEAPON_SIZE, weaponTextureComponent.texture.size.x, weaponTextureComponent.texture.size.y);
    weaponTextureComponent.zIndex = ZLayer::Weapon;
    componentManager.addComponent(weapon, weaponTextureComponent);

    AnimationComponent weaponAnimationComponent;
    weaponAnimationComponent.frame = 0;
    weaponAnimationComponent.time = 0;
    weaponAnimationComponent.frameSize = glm::vec2(WEAPON_SIZE, WEAPON_SIZE);
    weaponAnimationComponent.frameCount = std::map<State, int>{
        {State::Idle, 0},
        {State::Shooting, 6},
    };
    weaponAnimationComponent.stateDirectionRowMap = std::map<std::pair<State, Direction>, int>{
        {{State::Idle, Direction::South}, 0},    {{State::Idle, Direction::North}, 0},     {{State::Idle, Direction::East}, 0},
        {{State::Idle, Direction::West}, 0},     {{State::Shooting, Direction::South}, 0}, {{State::Shooting, Direction::North}, 0},
        {{State::Shooting, Direction::East}, 0}, {{State::Shooting, Direction::West}, 0},
    };
    weaponAnimationComponent.frameDuration = 0.1f;  // seconds
    weaponAnimationComponent.baseTextureCoords = glm::vec2(0, 0);
    weaponAnimationComponent.state = State::Idle;
    weaponAnimationComponent.loop = false;
    componentManager.addComponent(weapon, weaponAnimationComponent);

    RotationComponent rotationComponent;
    rotationComponent.angle = 180;
    componentManager.addComponent(weapon, rotationComponent);

    WeaponComponent weaponComponent;
    weaponComponent.range = 300;
    weaponComponent.damage = 25;
    weaponComponent.rateOfFire = 1.0f;
    weaponComponent.projectileSpeed = 300.0f;
    componentManager.addComponent(weapon, weaponComponent);

    // Link the weapon to the tower
    ChildComponent childComponent;
    childComponent.child = weapon;
    componentManager.addComponent(entity, childComponent);

    return entity;
}

Entity EntityFactory::createTowerProjectile(float x, float y, float targetX, float velocityX, float velocityY, float angle, int damage) {
    Entity entity = entityManager.createEntity();

    static const int TEXTURE_WIDTH = 8;
    static const int TEXTURE_HEIGHT = 40;
    static const int PROJECTILE_WIDTH = 8;
    static const int PROJECTILE_HEIGHT = 40;

    PositionComponent positionComponent;
    positionComponent.x = x - PROJECTILE_WIDTH / 2;
    positionComponent.y = y - PROJECTILE_HEIGHT / 2;
    componentManager.addComponent(entity, positionComponent);

    SizeComponent sizeComponent;
    sizeComponent.w = PROJECTILE_WIDTH;
    sizeComponent.h = PROJECTILE_HEIGHT;
    componentManager.addComponent(entity, sizeComponent);

    TextureComponent textureComponent;
    textureComponent.texture = textureManager.loadTexture("towers/tower1/Tower 01 - Level 01 - Projectile.png");
    textureComponent.coords = getTextureCoords(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT, textureComponent.texture.size.x, textureComponent.texture.size.y);
    textureComponent.zIndex = ZLayer::Projectile;
    componentManager.addComponent(entity, textureComponent);

    VelocityComponent velocityComponent;
    velocityComponent.x = velocityX;
    velocityComponent.y = velocityY;
    componentManager.addComponent(entity, velocityComponent);

    DamageComponent damageComponent;
    damageComponent.damage = damage;
    componentManager.addComponent(entity, damageComponent);

    AnimationComponent animationComponent;
    animationComponent.frame = 0;
    animationComponent.time = 0;
    animationComponent.frameSize = glm::vec2(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    animationComponent.frameCount = std::map<State, int>{
        {State::Active, 3},
    };
    animationComponent.frameDuration = 0.2f;  // seconds
    animationComponent.baseTextureCoords = glm::vec2(0, 0);
    animationComponent.state = State::Active;
    componentManager.addComponent(entity, animationComponent);

    RotationComponent rotationComponent;
    rotationComponent.angle = angle;
    componentManager.addComponent(entity, rotationComponent);

    // hitbox is the same as the texture
    CollisionComponent collisionComponent;
    collisionComponent.x = 0;
    collisionComponent.y = 0;
    collisionComponent.w = PROJECTILE_WIDTH;
    collisionComponent.h = PROJECTILE_HEIGHT;
    componentManager.addComponent(entity, collisionComponent);

    return entity;
}
