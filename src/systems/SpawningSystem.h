#pragma once
#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/EntityFactory.h"
#include "PathfindingSystem.h"

class SpawningSystem : public System {
public:
    SpawningSystem(ComponentManager& componentManager, EntityFactory& entityFactory, PathfindingSystem& pathfindingSystem)
        : componentManager(componentManager), entityFactory(entityFactory), pathfindingSystem(pathfindingSystem) {}

    void update(float deltaTime) override;
private:
    ComponentManager& componentManager;
    EntityFactory& entityFactory;
    PathfindingSystem& pathfindingSystem;

    float spawnTimer = 0.0f;
    float spawnInterval = 2.0f;
    const float MIN_SPAWN_INTERVAL = 0.35f;
};