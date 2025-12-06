#include "SpawningSystem.h"

#include <random>

#include "../components/ColorComponent.h"
#include "../components/DeathComponent.h"
#include "../components/PositionComponent.h"
#include "../components/SizeComponent.h"

void SpawningSystem::update(float deltaTime) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> randFactor(0.92f, 1.12f);
    static std::uniform_real_distribution<float> rareChance(0.0f, 1.0f);

    spawnTimer += deltaTime;

    if (spawnTimer >= spawnInterval) {
        Entity& start = pathfindingSystem.getStart();
        auto* position = componentManager.getComponent<PositionComponent>(start);
        auto* size = componentManager.getComponent<SizeComponent>(start);

        if (position && size) {
            ColorComponent colorComponent;

            float enemyHealth = healthStart * pow(1.01f, spawnCount);
            int enemySpeed = speedStart + spawnCount * 0.12f;
            int enemyGold = std::max(1, (int)(goldRewardStart * pow(0.999f, spawnCount)));

            enemyHealth *= randFactor(gen);
            enemySpeed = (int)(enemySpeed * randFactor(gen));

            bool redTrait  = rareChance(gen) < 0.018f;
            bool blueTrait = rareChance(gen) < 0.028f;

            glm::vec3 baseColor(1.0f, 1.0f, 1.0f);

            if (redTrait && blueTrait) {
                // Elite
                baseColor = {2.2f, 2.0f, 0.64f};
                enemyHealth *= 3.0f;
                enemySpeed = (int)(enemySpeed * 1.6f);
                enemyGold *= 6;
            } else if (redTrait) {
                // Tank
                baseColor = {1.8f, 0.6f, 0.6f};
                enemyHealth *= 2.2f;
                enemyGold *= 1.8f;
            } else if (blueTrait) {
                // Speed
                baseColor = {0.6f, 0.7f, 1.8f};
                enemySpeed = (int)(enemySpeed * 1.35f);
                enemyGold *= 2;
            }

            enemyHealth = std::min(enemyHealth, (float)maxHealth);
            enemySpeed = std::min(enemySpeed, maxSpeed);
            enemyGold  = std::min(enemyGold, maxGold);

            colorComponent.color = baseColor;

            auto enemy = entityFactory.createFireBug({position->x - size->w * 2, position->y}, enemyHealth, enemySpeed, enemyGold);
            componentManager.addComponent(enemy, colorComponent);

            spawnCount++;
        }

        spawnTimer -= spawnInterval;

        spawnInterval *= SPAWN_SCALE_FACTOR;

        if (spawnInterval < MIN_SPAWN_INTERVAL)
            spawnInterval = MIN_SPAWN_INTERVAL;
    }

    for (Entity entity : getEntities()) {
        auto* death = componentManager.getComponent<DeathComponent>(entity);
        if (death && death->hasDied) {
            if (death->remainingTime > 0) {
                death->remainingTime -= deltaTime;
            } else {
                componentManager.removeAllComponents(entity);
                entityManager->destroyEntity(entity);
            }
        }
    }
}

void SpawningSystem::onEvent(const Event& event) {
    auto& eventdispatcher = EventDispatcher::getInstance();

    if (event.type == EventType::SCHEDULE_REMOVAL) {
        Entity entity = *event.getData<Entity>("entity");

        auto* death = componentManager.getComponent<DeathComponent>(entity);
        if(death && !death->locked) {
            death->hasDied = true;
            death->remainingTime = 1.0f;
            death->locked = true;
        } else {
            DeathComponent death;
            death.hasDied = true;
            death.remainingTime = 1.0f;
            death.locked = true;
            componentManager.addComponent(entity, death);
        }
    }
}

