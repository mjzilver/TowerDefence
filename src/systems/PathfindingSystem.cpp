#include "PathfindingSystem.h"

#include <random>

#include "../components/DeathComponent.h"
#include "../components/PathfindingComponent.h"
#include "../components/PositionComponent.h"
#include "../components/SizeComponent.h"
#include "../components/SpeedComponent.h"
#include "../components/VelocityComponent.h"
#include "../ecs/Component.h"

PathfindingSystem::PathfindingSystem(ComponentManager& componentManager, MapLoader& mapLoader)
    : componentManager(componentManager), mapLoader(mapLoader), path(mapLoader.getPath()) {}

void PathfindingSystem::update(float) {
    auto* pathfinders = componentManager.getArray<PathfindingComponent>();
    auto* positions = componentManager.getArray<PositionComponent>();
    auto* sizes = componentManager.getArray<SizeComponent>();
    auto* velocities = componentManager.getArray<VelocityComponent>();
    auto* speeds = componentManager.getArray<SpeedComponent>();

    const float snapRadius = 3.0f;
    const float arrivalStart = 30.0f;
    const float minSpeed = 100.0f;
    const int lookAhead = 1;

    for (Entity entity : pathfinders->getEntities()) {
        auto* pos = positions->get(entity);
        auto* size = sizes->get(entity);
        auto* vel = velocities->get(entity);
        auto* pathFind = pathfinders->get(entity);
        auto* speed = speeds->get(entity);

        if (pathFind && pathFind->randomOffset.x == 0) {
            generateRandomOffset(pathFind, maxOffset);
        }

        if (pos && size && vel && speed && !pathFind->reachedGoal) {
            if (pathFind->currentIndex < 0 || pathFind->currentIndex >= (int)path.size()) continue;

            int targetIndex = std::min(pathFind->currentIndex + lookAhead, (int)path.size() - 1);
            const auto* targetPos = componentManager.getComponent<PositionComponent>(path[targetIndex].entity);
            const auto* targetSize = componentManager.getComponent<SizeComponent>(path[targetIndex].entity);

            if (targetPos && targetSize) {
                float ex = pos->x + size->w * 0.5f;
                float ey = pos->y + size->h * 0.5f;
                float tx = targetPos->x + targetSize->w * 0.5f + pathFind->randomOffset.x;
                float ty = targetPos->y + targetSize->h * 0.5f + pathFind->randomOffset.y;

                float dx = tx - ex;
                float dy = ty - ey;
                float dist = std::sqrt(dx * dx + dy * dy);

                if (dist <= snapRadius && pathFind->currentIndex < (int)path.size() - 1) {
                    pathFind->currentIndex++;
                    continue;
                }

                if (dist > 0.001f) {
                    dx /= dist;
                    dy /= dist;
                }

                float speedVal = speed->speed;
                bool isLastTile = (pathFind->currentIndex == (int)path.size() - 1);
                if (isLastTile && dist < arrivalStart) {
                    float t = (dist / arrivalStart);
                    speedVal = minSpeed + (speedVal - minSpeed) * (t * t);
                }

                vel->x = dx * speedVal;
                vel->y = dy * speedVal;

                if (isLastTile && dist <= snapRadius) {
                    pathFind->reachedGoal = true;
                    vel->x = vel->y = 0.0f;

                    DeathComponent deathComp;
                    deathComp.hasDied = true;
                    deathComp.remainingTime = 0.3f;
                    componentManager.addComponent(entity, deathComp);
                }
            }
        }
    }
}

void PathfindingSystem::generateRandomOffset(PathfindingComponent* pf, float maxOffset) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(-maxOffset, maxOffset);
    pf->randomOffset.x = dist(gen);
    pf->randomOffset.y = dist(gen);
}
