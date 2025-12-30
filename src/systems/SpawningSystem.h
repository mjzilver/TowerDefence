#pragma once
#include <glm/fwd.hpp>
#include <optional>

#include "../ecs/System.h"
#include "../event/Event.h"

class SpawningSystem : public System {
public:
    SpawningSystem(EngineContext& ctx);

    void onEvent(const Event& event);

    void update(const float deltaTime) override;

    void reset() override;

private:
    std::optional<glm::vec2> startDirection;

    void setStart();

    int spawnCount = 0;
    float spawnTimer = 0.0f;
    float spawnInterval = 3.0f;
    const float minSpawnInterval = 0.15f;
    const float spawnScaleFactor = 0.985f;

    const int initialHealth = 80;
    const int initialSpeed = 60;
    const int initialGold = 10;

    int healthStart = initialHealth;
    int speedStart = initialSpeed;
    int goldRewardStart = initialGold;
    float healthMultiplier = 1.0f;

    const int increaseHealthPer = 15;
    const int increaseSpeedPer = 20;
    const int increaseGoldPer = 15;

    const float healthStep = 0.15f;
    const int speedStep = 15;
    const int goldStep = 10;

    const int maxHealth = 1000;
    const int maxSpeed = 350;
    const int maxGold = 300;
};
