#pragma once
#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/EntityFactory.h"
#include "PathfindingSystem.h"

#include "../event/Event.h"

class SpawningSystem : public System {
public:
    SpawningSystem(ComponentManager& componentManager, EntityFactory& entityFactory, PathfindingSystem& pathfindingSystem)
        : componentManager(componentManager), entityFactory(entityFactory), pathfindingSystem(pathfindingSystem) {}

    void onEvent(const Event& event);

    void update(float deltaTime) override;

private:
    ComponentManager& componentManager;
    EntityFactory& entityFactory;
    PathfindingSystem& pathfindingSystem;

    int spawnCount = 0;
    float spawnTimer = 0.0f;
    float spawnInterval = 3.0f;
    const float MIN_SPAWN_INTERVAL = 0.25f;
    const float SPAWN_SCALE_FACTOR = 0.983f;

    int healthStart = 80;
    int speedStart = 60;
    int goldRewardStart = 10;

    int increaseHealthPer = 10;
    int increaseSpeedPer = 20;
    int increaseGoldPer = 15;

    float healthStep = 0.25f;
    int speedStep = 15;
    int goldStep = 10;

    float healthMultiplier = 1.0f;

    int maxHealth = 1000;
    int maxSpeed = 240;
    int maxGold = 300;
};
