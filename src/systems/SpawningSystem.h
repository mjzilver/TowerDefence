#pragma once
#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/EntityFactory.h"
#include "PathfindingSystem.h"

#include "../event/EventDispatcher.h"
#include "../event/Event.h"

class SpawningSystem : public System {
public:
    SpawningSystem(ComponentManager& componentManager, EntityFactory& entityFactory, PathfindingSystem& pathfindingSystem)
        : componentManager(componentManager), entityFactory(entityFactory), pathfindingSystem(pathfindingSystem) {
            EventDispatcher::getInstance().addListener(EventType::SCHEDULE_REMOVAL, std::bind(&SpawningSystem::onEvent, this, std::placeholders::_1));
    }

    void onEvent(const Event& event);

    void update(float deltaTime) override;

private:
    ComponentManager& componentManager;
    EntityFactory& entityFactory;
    PathfindingSystem& pathfindingSystem;

    float spawnTimer = -3.0f;
    float spawnInterval = 2.75f;
    const float MIN_SPAWN_INTERVAL = 0.35f;

    const float SPAWN_SCALE_FACTOR = 0.995f; 

    int spawnCount = 0;

    int healthStart = 100;
    int speedStart = 80;
    int goldRewardStart = 5;

    int increaseHealthPer = 5;
    int increaseSpeedPer = 7;
    int increaseGoldPer = 15;

    float healthStep = 0.1f;
    int speedStep = 5;
    int goldStep = 1;

    float healthMultiplier = 1.0f; 
    float redShift = 1.0f;

    int maxHealth = 500;
    int maxSpeed = 300;
    int maxGold = 35;

    float maxRedShift = 1.8f;
};
