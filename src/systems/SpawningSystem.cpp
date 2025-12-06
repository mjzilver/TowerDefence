#include "SpawningSystem.h"

#include "../components/DeathComponent.h"
#include "../components/PositionComponent.h"
#include "../components/SizeComponent.h"

void SpawningSystem::update(float deltaTime) {
    spawnTimer += deltaTime;

    if (spawnTimer >= spawnInterval) {
        Entity& start = pathfindingSystem.getStart();
        auto* position = componentManager.getComponent<PositionComponent>(start);
        auto* size = componentManager.getComponent<SizeComponent>(start);

        if (position && size) {
            Entity enemy = entityFactory.createFireBug({position->x - size->w * 2, position->y});
        }

        spawnTimer -= spawnInterval;

        spawnInterval *= 0.98f;
        if (spawnInterval < MIN_SPAWN_INTERVAL) spawnInterval = MIN_SPAWN_INTERVAL;
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