#include "EntityFactory.h"

#include <utility>

#include "../components/AnimationComponent.h"
#include "../components/ChildComponent.h"
#include "../components/ClickableComponent.h"
#include "../components/CollisionComponent.h"
#include "../components/ColorComponent.h"
#include "../components/DamageComponent.h"
#include "../components/DeathComponent.h"
#include "../components/DirectionComponent.h"
#include "../components/FlagComponent.h"
#include "../components/HealthComponent.h"
#include "../components/PathfindingComponent.h"
#include "../components/PositionComponent.h"
#include "../components/RewardComponent.h"
#include "../components/RotationComponent.h"
#include "../components/ShaderComponent.h"
#include "../components/SizeComponent.h"
#include "../components/SpeedComponent.h"
#include "../components/TextComponent.h"
#include "../components/TextureComponent.h"
#include "../components/UpgradeComponent.h"
#include "../components/VelocityComponent.h"
#include "../components/WeaponComponent.h"
#include "../event/Event.h"
#include "../utils/Color.h"
#include "../utils/Globals.h"
#include "../utils/State.h"
#include "../utils/TextureCoords.h"
#include "../utils/ZLayer.h"

EntityFactory::EntityFactory(ComponentManager& componentManager, EntityManager& entityManager, TextureManager& textureManager)
    : componentManager(componentManager), entityManager(entityManager), textureManager(textureManager) {
    textureManager.preload({
        "tiles/Grass.png",
        "enemy/Firebug.png",
        "towers/tower1/Tower 01.png",
        "towers/tower1/Tower 01 - Level 01 - Weapon.png",
        "towers/tower1/Tower 01 - Level 01 - Projectile.png",
        "towers/tower1/Tower 01 - Weapon - Impact.png",
    });
}

Entity EntityFactory::createGrassTile(const glm::vec2& position) {
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
    textureComponent.texture = textureManager.get("tiles/Grass.png");
    textureComponent.coords = getTextureCoords(2, 1, TEXTURE_SIZE, TEXTURE_SIZE, textureComponent.texture.size.x, textureComponent.texture.size.y);
    componentManager.addComponent(entity, textureComponent);

    ClickableComponent clickableComponent;
    clickableComponent.clickedEvent = EventType::GRASS_TILE_CLICKED;
    componentManager.addComponent(entity, clickableComponent);

    ShaderComponent shaderComponent;
    shaderComponent.name = "default";
    componentManager.addComponent(entity, shaderComponent);

    return entity;
}

Entity EntityFactory::createPathTile(const glm::vec2& position) {
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
    flagComponent.type = FlagType::PATH;
    componentManager.addComponent(entity, flagComponent);

    TextureComponent textureComponent;
    textureComponent.texture = textureManager.get("tiles/Grass.png");
    textureComponent.coords = getTextureCoords(9, 2, TEXTURE_SIZE, TEXTURE_SIZE, textureComponent.texture.size.x, textureComponent.texture.size.y);
    componentManager.addComponent(entity, textureComponent);

    return entity;
}

Entity EntityFactory::createStartTile(const glm::vec2& position) {
    Entity entity = createPathTile(position);
    FlagComponent flagComponent;
    flagComponent.type = START;
    componentManager.addComponent(entity, flagComponent);
    return entity;
}

Entity EntityFactory::createEndTile(const glm::vec2& position) {
    Entity entity = createPathTile(position);
    FlagComponent flagComponent;
    flagComponent.type = END;
    componentManager.addComponent(entity, flagComponent);
    return entity;
}

Entity EntityFactory::createFireBug(const glm::vec2& position, int health, int speed, int goldReward) {
    Entity entity = entityManager.createEntity();

    static const int TEXTURE_WIDTH = 128;
    static const int TEXTURE_HEIGHT = 64;
    static const int ENEMY_WIDTH = 128;
    static const int ENEMY_HEIGHT = 64;

    PositionComponent positionComponent;
    positionComponent.x = position.x;
    positionComponent.y = position.y;
    positionComponent.zIndex = ZLayer::ENEMY;
    componentManager.addComponent(entity, positionComponent);

    VelocityComponent velocityComponent;
    velocityComponent.x = 0;
    velocityComponent.y = 0;
    componentManager.addComponent(entity, velocityComponent);

    HealthComponent healthComponent;
    healthComponent.health = health;
    componentManager.addComponent(entity, healthComponent);

    DeathComponent deathComponent;
    deathComponent.remainingTime = 30.0f;
    componentManager.addComponent(entity, deathComponent);

    RewardComponent rewardComponent;
    rewardComponent.gold = goldReward;
    componentManager.addComponent(entity, rewardComponent);

    SpeedComponent speedComponent;
    speedComponent.speed = speed;
    componentManager.addComponent(entity, speedComponent);

    SizeComponent sizeComponent;
    sizeComponent.w = ENEMY_WIDTH;
    sizeComponent.h = ENEMY_HEIGHT;
    componentManager.addComponent(entity, sizeComponent);

    ClickableComponent clickableComponent;
    clickableComponent.clickedEvent = EventType::ENTITY_CLICKED;
    componentManager.addComponent(entity, clickableComponent);

    TextureComponent textureComponent;
    textureComponent.texture = textureManager.get("enemy/Firebug.png");
    textureComponent.coords = getTextureCoords(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT, textureComponent.texture.size.x, textureComponent.texture.size.y);
    componentManager.addComponent(entity, textureComponent);

    DirectionComponent directionComponent;
    directionComponent.direction = Direction::SOUTH;
    componentManager.addComponent(entity, directionComponent);

    AnimationComponent animationComponent;
    animationComponent.frame = 0;
    animationComponent.time = 0;
    animationComponent.frameSize = glm::vec2(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    animationComponent.frameCount = std::map<State, int>{
        {State::IDLE, 6},
        {State::ACTIVE, 8},
        {State::DEAD, 11},
    };
    animationComponent.stateDirectionRowMap = std::map<std::pair<State, Direction>, int>{
        {{State::IDLE, Direction::SOUTH}, 0},  {{State::IDLE, Direction::NORTH}, 1},   {{State::IDLE, Direction::EAST}, 2},
        {{State::IDLE, Direction::WEST}, 2},   {{State::ACTIVE, Direction::SOUTH}, 3}, {{State::ACTIVE, Direction::NORTH}, 4},
        {{State::ACTIVE, Direction::EAST}, 5}, {{State::ACTIVE, Direction::WEST}, 5},  {{State::DEAD, Direction::SOUTH}, 6},
        {{State::DEAD, Direction::NORTH}, 7},  {{State::DEAD, Direction::EAST}, 8},    {{State::DEAD, Direction::WEST}, 8},
    };
    animationComponent.frameDuration = 0.2f;  // seconds
    animationComponent.baseTextureCoords = glm::vec2(0, 0);
    animationComponent.state = State::ACTIVE;
    componentManager.addComponent(entity, animationComponent);

    PathfindingComponent pathfindingComponent;
    pathfindingComponent.x = 0;
    pathfindingComponent.y = 0;
    componentManager.addComponent(entity, pathfindingComponent);

    const float hitboxSize = 48;
    CollisionComponent collisionComponent;
    collisionComponent.w = hitboxSize;
    collisionComponent.h = hitboxSize;
    collisionComponent.x = (ENEMY_WIDTH - hitboxSize) / 2.0f;
    collisionComponent.y = (ENEMY_HEIGHT - hitboxSize) / 2.0f;
    componentManager.addComponent(entity, collisionComponent);

    return entity;
}

Entity EntityFactory::createTower(const glm::vec2& position) {
    Entity entity = entityManager.createEntity();

    static const int TEXTURE_WIDTH = 64;
    static const int TEXTURE_HEIGHT = 128;
    static const int TOWER_WIDTH = 64;
    static const int TOWER_HEIGHT = 128;

    PositionComponent positionComponent;
    positionComponent.x = position.x + 8;
    positionComponent.y = position.y + 8;
    positionComponent.zIndex = ZLayer::TOWER;
    componentManager.addComponent(entity, positionComponent);

    SizeComponent sizeComponent;
    sizeComponent.w = TOWER_WIDTH;
    sizeComponent.h = TOWER_HEIGHT;
    componentManager.addComponent(entity, sizeComponent);

    UpgradeComponent upgradeComponent;
    upgradeComponent.maxUpgradeLevel = 3;
    componentManager.addComponent(entity, upgradeComponent);

    TextureComponent textureComponent;
    textureComponent.texture = textureManager.get("towers/tower1/Tower 01.png");
    textureComponent.coords = getTextureCoords(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT, textureComponent.texture.size.x, textureComponent.texture.size.y);
    componentManager.addComponent(entity, textureComponent);

    Entity weapon = entityManager.createEntity();

    const int weaponSize = 96;

    PositionComponent weaponPositionComponent;
    weaponPositionComponent.x = position.x + 8 + (TOWER_WIDTH - weaponSize) / 2;
    weaponPositionComponent.y = position.y + 8;
    weaponPositionComponent.zIndex = ZLayer::WEAPON;
    componentManager.addComponent(weapon, weaponPositionComponent);

    SizeComponent weaponSizeComponent;
    weaponSizeComponent.w = weaponSize;
    weaponSizeComponent.h = weaponSize;
    componentManager.addComponent(weapon, weaponSizeComponent);

    TextureComponent weaponTextureComponent;
    weaponTextureComponent.texture = textureManager.get("towers/tower1/Tower 01 - Level 01 - Weapon.png");
    weaponTextureComponent.coords =
        getTextureCoords(0, 0, weaponSize, weaponSize, weaponTextureComponent.texture.size.x, weaponTextureComponent.texture.size.y);
    componentManager.addComponent(weapon, weaponTextureComponent);

    AnimationComponent weaponAnimationComponent;
    weaponAnimationComponent.frame = 0;
    weaponAnimationComponent.time = 0;
    weaponAnimationComponent.frameSize = glm::vec2(weaponSize, weaponSize);
    weaponAnimationComponent.frameCount = std::map<State, int>{
        {State::IDLE, 0},
        {State::SHOOTING, 6},
    };
    weaponAnimationComponent.stateDirectionRowMap = std::map<std::pair<State, Direction>, int>{
        {{State::IDLE, Direction::SOUTH}, 0},    {{State::IDLE, Direction::NORTH}, 0},     {{State::IDLE, Direction::EAST}, 0},
        {{State::IDLE, Direction::WEST}, 0},     {{State::SHOOTING, Direction::SOUTH}, 0}, {{State::SHOOTING, Direction::NORTH}, 0},
        {{State::SHOOTING, Direction::EAST}, 0}, {{State::SHOOTING, Direction::WEST}, 0},
    };
    weaponAnimationComponent.frameDuration = 0.1f;  // seconds
    weaponAnimationComponent.baseTextureCoords = glm::vec2(0, 0);
    weaponAnimationComponent.state = State::IDLE;
    weaponAnimationComponent.loop = false;
    componentManager.addComponent(weapon, weaponAnimationComponent);

    RotationComponent rotationComponent;
    rotationComponent.angle = 180;
    componentManager.addComponent(weapon, rotationComponent);

    WeaponComponent weaponComponent;
    weaponComponent.range = 250;
    weaponComponent.damage = 25;
    weaponComponent.rateOfFire = 1.0f;
    weaponComponent.projectileSpeed = 300.0f;
    componentManager.addComponent(weapon, weaponComponent);

    // Link the weapon to the tower
    ChildComponent childComponent;
    childComponent.child = weapon;
    componentManager.addComponent(entity, childComponent);

    ClickableComponent clickableComponent;
    clickableComponent.clickedEvent = EventType::TOWER_CLICKED;
    componentManager.addComponent(entity, clickableComponent);

    ShaderComponent shaderComponent;
    shaderComponent.name = "default";
    componentManager.addComponent(entity, shaderComponent);

    return entity;
}

Entity EntityFactory::createTowerProjectile(float x, float y, float /*targetX*/, float velocityX, float velocityY, float angle, int damage) {
    Entity entity = entityManager.createEntity();

    static const int TEXTURE_WIDTH = 8;
    static const int TEXTURE_HEIGHT = 40;
    static const int PROJECTILE_WIDTH = 8;
    static const int PROJECTILE_HEIGHT = 40;

    PositionComponent positionComponent;
    positionComponent.x = x - PROJECTILE_WIDTH / 2;
    positionComponent.y = y - PROJECTILE_HEIGHT / 2;
    positionComponent.zIndex = ZLayer::PROJECTILE;
    componentManager.addComponent(entity, positionComponent);

    SizeComponent sizeComponent;
    sizeComponent.w = PROJECTILE_WIDTH;
    sizeComponent.h = PROJECTILE_HEIGHT;
    componentManager.addComponent(entity, sizeComponent);

    TextureComponent textureComponent;
    textureComponent.texture = textureManager.get("towers/tower1/Tower 01 - Level 01 - Projectile.png");
    textureComponent.coords = getTextureCoords(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT, textureComponent.texture.size.x, textureComponent.texture.size.y);
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
        {State::ACTIVE, 3},
    };
    animationComponent.frameDuration = 0.2f;  // seconds
    animationComponent.baseTextureCoords = glm::vec2(0, 0);
    animationComponent.state = State::ACTIVE;
    componentManager.addComponent(entity, animationComponent);

    RotationComponent rotationComponent;
    rotationComponent.angle = angle;
    componentManager.addComponent(entity, rotationComponent);

    CollisionComponent collisionComponent;
    collisionComponent.x = -PROJECTILE_WIDTH;
    collisionComponent.y = 0;
    collisionComponent.w = PROJECTILE_WIDTH * 3;
    collisionComponent.h = PROJECTILE_HEIGHT;
    componentManager.addComponent(entity, collisionComponent);

    return entity;
}

Entity EntityFactory::createTowerProjectileImpact(glm::vec2 position) {
    Entity entity = entityManager.createEntity();

    static const int TEXTURE_WIDTH = 64;
    static const int TEXTURE_HEIGHT = 64;

    PositionComponent positionComponent;
    positionComponent.x = position.x - TEXTURE_WIDTH / 2;
    positionComponent.y = position.y - TEXTURE_HEIGHT / 2;
    positionComponent.zIndex = ZLayer::PARTICLE;
    componentManager.addComponent(entity, positionComponent);

    SizeComponent sizeComponent;
    sizeComponent.w = TEXTURE_WIDTH;
    sizeComponent.h = TEXTURE_HEIGHT;
    componentManager.addComponent(entity, sizeComponent);

    TextureComponent textureComponent;
    textureComponent.texture = textureManager.get("towers/tower1/Tower 01 - Weapon - Impact.png");
    textureComponent.coords = getTextureCoords(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT, textureComponent.texture.size.x, textureComponent.texture.size.y);
    componentManager.addComponent(entity, textureComponent);

    AnimationComponent animationComponent;
    animationComponent.frame = 0;
    animationComponent.time = 0;
    animationComponent.frameSize = glm::vec2(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    animationComponent.frameCount = std::map<State, int>{
        {State::ACTIVE, 6},
    };
    animationComponent.frameDuration = 0.1f;  // seconds
    animationComponent.baseTextureCoords = glm::vec2(0, 0);
    animationComponent.state = State::ACTIVE;
    animationComponent.loop = false;
    animationComponent.removeAtEnd = true;
    componentManager.addComponent(entity, animationComponent);

    return entity;
}

Entity EntityFactory::createRectangle(const glm::vec2& position, const glm::vec2& size) {
    Entity entity = entityManager.createEntity();

    PositionComponent positionComponent;
    positionComponent.x = position.x;
    positionComponent.y = position.y;
    positionComponent.zIndex = ZLayer::UI_BACKGROUND;
    componentManager.addComponent(entity, positionComponent);

    SizeComponent sizeComponent;
    sizeComponent.w = size.x;
    sizeComponent.h = size.y;
    componentManager.addComponent(entity, sizeComponent);

    ColorComponent colorComponent;
    colorComponent.color = {0.33f, 0.33f, 0.33f};
    componentManager.addComponent(entity, colorComponent);

    ShaderComponent shaderComponent;
    shaderComponent.name = "rect";
    componentManager.addComponent(entity, shaderComponent);

    return entity;
}

Entity EntityFactory::createKillCounter(const glm::vec2& pos) {
    Entity entity = entityManager.createEntity();

    PositionComponent positionComponent;
    positionComponent.x = pos.x;
    positionComponent.y = pos.y;
    positionComponent.zIndex = ZLayer::UI_ELEMENT;
    componentManager.addComponent(entity, positionComponent);

    SizeComponent sizeComponent;
    sizeComponent.w = 100;
    sizeComponent.h = 75;
    componentManager.addComponent(entity, sizeComponent);

    TextComponent textComponent;
    textComponent.text = "Kills\n???";
    textComponent.color = {0.1f, 0.1f, 0.1f};
    componentManager.addComponent(entity, textComponent);

    ColorComponent colorComponent;
    colorComponent.color = SOFT_RED;
    componentManager.addComponent(entity, colorComponent);

    ShaderComponent shaderComponent;
    shaderComponent.name = "text";
    componentManager.addComponent(entity, shaderComponent);

    return entity;
}

Entity EntityFactory::createUpgradeMenuItem(const glm::vec2& pos, std::function<void()> onClick, int cost) {
    Entity entity = entityManager.createEntity();

    PositionComponent positionComponent;
    positionComponent.x = pos.x;
    positionComponent.y = pos.y;
    positionComponent.zIndex = ZLayer::UI_ELEMENT;
    componentManager.addComponent(entity, positionComponent);

    SizeComponent sizeComponent;
    sizeComponent.w = 100;
    sizeComponent.h = 75;
    componentManager.addComponent(entity, sizeComponent);

    TextComponent textComponent;
    textComponent.text = "Upgrade\n" + std::to_string(cost);

    textComponent.color = {0.1f, 0.1f, 0.1f};
    componentManager.addComponent(entity, textComponent);

    ClickableComponent clickableComponent;
    clickableComponent.type = ClickableType::FUNCTION;
    clickableComponent.onClick = std::move(onClick);
    componentManager.addComponent(entity, clickableComponent);

    ColorComponent colorComponent;
    colorComponent.color = SOFT_ORANGE;
    componentManager.addComponent(entity, colorComponent);

    ShaderComponent shaderComponent;
    shaderComponent.name = "text";
    componentManager.addComponent(entity, shaderComponent);

    return entity;
}

Entity EntityFactory::createBuildTowerMenuItem(const glm::vec2& pos, std::function<void()> onClick) {
    Entity entity = entityManager.createEntity();

    PositionComponent positionComponent;
    positionComponent.x = pos.x;
    positionComponent.y = pos.y;
    positionComponent.zIndex = ZLayer::UI_ELEMENT;
    componentManager.addComponent(entity, positionComponent);

    SizeComponent sizeComponent;
    sizeComponent.w = 100;
    sizeComponent.h = 75;
    componentManager.addComponent(entity, sizeComponent);

    TextComponent textComponent;
    textComponent.text = "Build Tower\n50 gold";
    textComponent.color = {0.1f, 0.1f, 0.1f};
    componentManager.addComponent(entity, textComponent);

    ClickableComponent clickableComponent;
    clickableComponent.type = ClickableType::FUNCTION;
    clickableComponent.onClick = std::move(onClick);
    componentManager.addComponent(entity, clickableComponent);

    ColorComponent colorComponent;
    colorComponent.color = LILAC;
    componentManager.addComponent(entity, colorComponent);

    ShaderComponent shaderComponent;
    shaderComponent.name = "text";
    componentManager.addComponent(entity, shaderComponent);

    return entity;
}

Entity EntityFactory::createCurrencyDisplay(const glm::vec2& pos) {
    Entity entity = entityManager.createEntity();

    PositionComponent positionComponent;
    positionComponent.x = pos.x;
    positionComponent.y = pos.y;
    positionComponent.zIndex = ZLayer::UI_ELEMENT;
    componentManager.addComponent(entity, positionComponent);

    SizeComponent sizeComponent;
    sizeComponent.w = 100;
    sizeComponent.h = 75;
    componentManager.addComponent(entity, sizeComponent);

    TextComponent textComponent;
    textComponent.text = "??? gold";
    textComponent.color = {0.1f, 0.1f, 0.1f};
    componentManager.addComponent(entity, textComponent);

    ColorComponent colorComponent;
    colorComponent.color = SOFT_YELLOW;
    componentManager.addComponent(entity, colorComponent);

    ShaderComponent shaderComponent;
    shaderComponent.name = "text";
    componentManager.addComponent(entity, shaderComponent);

    return entity;
}

Entity EntityFactory::createMenuButton(const glm::vec2& pos, std::function<void()> onClick) {
    Entity entity = entityManager.createEntity();

    PositionComponent positionComponent;
    positionComponent.x = pos.x;
    positionComponent.y = pos.y;
    positionComponent.zIndex = ZLayer::UI_ELEMENT;
    componentManager.addComponent(entity, positionComponent);

    SizeComponent sizeComponent;
    sizeComponent.w = 100;
    sizeComponent.h = 75;
    componentManager.addComponent(entity, sizeComponent);

    TextComponent textComponent;
    textComponent.text = "Menu";
    textComponent.color = {0.1f, 0.1, 0.1f};
    componentManager.addComponent(entity, textComponent);

    ClickableComponent clickableComponent;
    clickableComponent.type = ClickableType::FUNCTION;
    clickableComponent.onClick = std::move(onClick);
    componentManager.addComponent(entity, clickableComponent);

    ColorComponent colorComponent;
    colorComponent.color = MUTED_PURPLE;
    componentManager.addComponent(entity, colorComponent);

    ShaderComponent shaderComponent;
    shaderComponent.name = "text";
    componentManager.addComponent(entity, shaderComponent);

    return entity;
}

Entity EntityFactory::upgradeTower(Entity entity) {
    auto* upgradeComponent = componentManager.getComponent<UpgradeComponent>(entity);
    upgradeComponent->upgradeLevel++;

    static const int TEXTURE_WIDTH = 64;
    static const int TEXTURE_HEIGHT = 128;
    static const int TOWER_WIDTH = 64;
    static const int TOWER_HEIGHT = 128;

    auto* textureComponent = componentManager.getComponent<TextureComponent>(entity);
    textureComponent->coords = getTextureCoords(upgradeComponent->upgradeLevel - 1, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT,
                                                textureComponent->texture.size.x, textureComponent->texture.size.y);

    auto* childComponent = componentManager.getComponent<ChildComponent>(entity);
    auto weaponEntity = childComponent->child;

    auto* weaponPosition = componentManager.getComponent<PositionComponent>(weaponEntity);
    weaponPosition->y -= 8;

    auto* weaponComponent = componentManager.getComponent<WeaponComponent>(weaponEntity);
    weaponComponent->damage += 10;
    weaponComponent->projectileSpeed *= 1.2f;
    weaponComponent->rateOfFire *= 0.8f;
    weaponComponent->range *= 1.2f;

    return entity;
}
