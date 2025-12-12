#pragma once
#include <glm/fwd.hpp>
#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/EntityFactory.h"
#include "../map/MapLoader.h"

#include "../event/Event.h"

class SpawningSystem : public System {
public:
    SpawningSystem(ComponentManager& componentManager, EntityFactory& entityFactory, MapLoader& mapLoader);

    void onEvent(const Event& event);

    void update(float deltaTime) override;

private:
    ComponentManager& componentManager;
    EntityFactory& entityFactory;
    MapLoader& mapLoader;

    glm::vec2 startDirection;

    int spawnCount = 0;
    float spawnTimer = 0.0f;
    float spawnInterval = 3.0f;
    const float minSpawnInterval = 0.25f;
    const float spawnScaleFactor = 0.985f;

    int healthStart = 80;
    int speedStart = 60;
    int goldRewardStart = 10;

    int increaseHealthPer = 15;
    int increaseSpeedPer = 20;
    int increaseGoldPer = 15;

    float healthStep = 0.15f;
    int speedStep = 15;
    int goldStep = 10;

    float healthMultiplier = 1.0f;

    int maxHealth = 1000;
    int maxSpeed = 240;
    int maxGold = 300;
};
