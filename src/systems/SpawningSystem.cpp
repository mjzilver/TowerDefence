#include "SpawningSystem.h"

#include <random>

#include "../components/ColorComponent.h"
#include "../components/DeathComponent.h"
#include "../components/PositionComponent.h"
#include "../components/SizeComponent.h"
#include "../utils/Globals.h"

SpawningSystem::SpawningSystem(ComponentManager& componentManager, EntityFactory& entityFactory, MapLoader& mapLoader)
    : componentManager(componentManager), entityFactory(entityFactory), mapLoader(mapLoader) {
#if STRESS_TEST
    spawnCount = 100000;
    spawnInterval = minSpawnInterval;
#endif

    auto& path = mapLoader.getPath();

    startDirection = {path[0].x - path[1].x, path[0].y - path[1].y};

    endDirection = {
        path[path.size() - 1].x - path[path.size() - 2].x,
        path[path.size() - 1].y - path[path.size() - 2].y,
    };
}

void SpawningSystem::update(float deltaTime) {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    static std::uniform_real_distribution<float> healthRand(0.92f, 1.08f);
    static std::uniform_real_distribution<float> speedRand(0.90f, 1.10f);
    static std::uniform_real_distribution<float> goldRand(0.88f, 1.12f);
    static std::uniform_real_distribution<float> rareChance(0.0f, 1.0f);

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

        Entity start = mapLoader.getStart().entity;
        auto* position = componentManager.getComponent<PositionComponent>(start);
        auto* size = componentManager.getComponent<SizeComponent>(start);

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

            auto enemy = entityFactory.createFireBug({position->x - (size->w * 2) * startDirection.x, position->y + (size->h * 2) * startDirection.y},
                                                     enemyHealth, enemySpeed, enemyGold);
            componentManager.addComponent(enemy, colorComponent);

            spawnCount++;
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