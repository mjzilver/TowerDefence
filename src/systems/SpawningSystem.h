#pragma once
#include <glm/fwd.hpp>
#include <optional>

#include "../ecs/ComponentManager.h"
#include "../ecs/EntityFactory.h"
#include "../ecs/System.h"
#include "../event/Event.h"
#include "../map/MapLoader.h"

class SpawningSystem : public System {
public:
    SpawningSystem(ComponentManager& componentManager, EntityFactory& entityFactory, MapLoader& mapLoader);

    void onEvent(const Event& event);

    void update(float deltaTime) override;

    void reset() override;

private:
    ComponentManager& componentManager;
    EntityFactory& entityFactory;
    MapLoader& mapLoader;

    std::optional<glm::vec2> startDirection;

    void setStart();

    int spawnCount = 0;
    float spawnTimer = 0.0f;
    float spawnInterval = 3.0f;
    const float minSpawnInterval = 0.25f;
    const float spawnScaleFactor = 0.985f;

    int healthStart = 80;
    int speedStart = 60;
    int goldRewardStart = 10;
    float healthMultiplier = 1.0f;

    const int increaseHealthPer = 15;
    const int increaseSpeedPer = 20;
    const int increaseGoldPer = 15;

    const float healthStep = 0.15f;
    const int speedStep = 15;
    const int goldStep = 10;

    const int maxHealth = 1000;
    const int maxSpeed = 240;
    const int maxGold = 300;
};
