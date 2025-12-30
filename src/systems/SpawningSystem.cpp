#include "SpawningSystem.h"

#include <iostream>
#include <optional>
#include <random>

#include "../components/ColorComponent.h"
#include "../components/DeathComponent.h"
#include "../components/PositionComponent.h"
#include "../components/SizeComponent.h"
#include "../event/Event.h"
#include "../event/EventDispatcher.h"
#include "../utils/Globals.h"

SpawningSystem::SpawningSystem(EngineContext& ctx) : System(ctx) {
    context.eventDispatcher.addListener(EventType::ACTIVATE_STRESS_TEST, std::bind(&SpawningSystem::onEvent, this, std::placeholders::_1));

    writes.push_back(typeid(DeathComponent));
}

void SpawningSystem::setStart() {
    auto& path = context.mapLoader.getPath();

    if (path.size() == 0) {
        return;
    }

    const glm::vec2 dir = glm::vec2(path[1].x - path[0].x, path[1].y - path[0].y);

    startDirection = glm::vec2((dir.x > 0) ? 1 : (dir.x < 0 ? -1 : 0), (dir.y > 0) ? 1 : (dir.y < 0 ? -1 : 0));
}

void SpawningSystem::reset() {
    spawnCount = 0;
    spawnTimer = 0.0f;
    spawnInterval = 3.0f;

    healthStart = initialHealth;
    speedStart = initialSpeed;
    goldRewardStart = initialGold;
    healthMultiplier = 1.0f;

    startDirection = std::nullopt;
}

void SpawningSystem::update(float deltaTime) {
    if (!startDirection.has_value()) {
        setStart();
        return;
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());

    static std::uniform_real_distribution<float> healthRand(0.92f, 1.08f);
    static std::uniform_real_distribution<float> speedRand(0.90f, 1.10f);
    static std::uniform_real_distribution<float> goldRand(0.88f, 1.12f);
    static std::uniform_real_distribution<float> rareChance(0.0f, 1.0f);

    auto& componentManager = context.componentManager;

    spawnTimer += deltaTime;

    if (spawnTimer >= spawnInterval) {
        if (spawnCount > 0) {
            if (spawnCount % increaseHealthPer == 0) {
                healthMultiplier += healthStep;
            }
            if (spawnCount % increaseSpeedPer == 0) {
                speedStart += speedStep;
            }
            if (spawnCount % increaseGoldPer == 0) {
                goldRewardStart += goldStep;
            }
        }

        Entity start = context.mapLoader.getStart().entity;

        const auto* position = componentManager.getComponent<PositionComponent>(start);
        const auto* size = componentManager.getComponent<SizeComponent>(start);

        if (position && size) {
            ColorComponent colorComponent;

            float enemyHealth = (healthStart * healthMultiplier);
            int enemySpeed = speedStart;
            int enemyGold = goldRewardStart;

            enemyHealth *= healthRand(gen);
            enemySpeed = (int)(enemySpeed * speedRand(gen));
            enemyGold = (int)(enemyGold * goldRand(gen));

            bool redTrait = rareChance(gen) < 0.015f;
            bool blueTrait = rareChance(gen) < 0.020f;

            glm::vec3 baseColor(1.0f, 1.0f, 1.0f);

            if (redTrait && blueTrait) {
                // Elite
                baseColor = {2.2f, 2.0f, 0.64f};
                enemyHealth *= 2.8f;
                enemySpeed = (int)(enemySpeed * 1.45f);
                enemyGold *= 4;
            } else if (redTrait) {
                // Tank
                baseColor = {1.8f, 0.6f, 0.6f};
                enemyHealth *= 1.9f;
                enemyGold *= 1.7f;
            } else if (blueTrait) {
                // Speed
                baseColor = {0.6f, 0.7f, 1.8f};
                enemySpeed = (int)(enemySpeed * 1.30f);
                enemyGold *= 1.5f;
            }

            // Clamp so late game doesn't become impossible
            enemyHealth = std::min(enemyHealth, (float)maxHealth);
            enemySpeed = std::min(enemySpeed, maxSpeed);
            enemyGold = std::min(enemyGold, maxGold);

            colorComponent.color = baseColor;

            const glm::vec2 spawnPos = glm::vec2(position->x - startDirection->x * TILE_SIZE, position->y - startDirection->y * TILE_SIZE);

            auto enemy = context.entityFactory.createFireBug(spawnPos, enemyHealth, enemySpeed, enemyGold);
            componentManager.addComponent(enemy, colorComponent);

            spawnCount++;
        } else {
            std::cerr << "start tile is invalid\n";
        }

        spawnTimer -= spawnInterval;

        spawnInterval *= spawnScaleFactor;
        if (spawnInterval < minSpawnInterval) spawnInterval = minSpawnInterval;
    }

    auto* deaths = componentManager.getArray<DeathComponent>();

    for (Entity entity : deaths->getEntities()) {
        auto* death = deaths->get(entity);
        if (!death) continue;

        if (death->hasDied) {
            if (death->remainingTime > 0) {
                death->remainingTime -= deltaTime;
            } else {
                componentManager.scheduleDestruction(entity);
            }
        }
    }
}

void SpawningSystem::onEvent(const Event& event) {
    if (event.type == EventType::ACTIVATE_STRESS_TEST) {
        spawnCount = 100000;
        spawnTimer = 0.0f;
        spawnInterval = minSpawnInterval;

        healthStart = maxHealth;
        speedStart = maxSpeed;
        goldRewardStart = maxGold;
        healthMultiplier = maxHealth;
    }
};