#include "PathfindingSystem.h"

#include <random>

#include "../components/DeathComponent.h"
#include "../components/PathfindingComponent.h"
#include "../components/PositionComponent.h"
#include "../components/SizeComponent.h"
#include "../components/SpeedComponent.h"
#include "../components/VelocityComponent.h"
#include "../ecs/Component.h"

PathfindingSystem::PathfindingSystem(EngineContext& ctx) : System(ctx), path(context.mapLoader.getPath()) {
    writes.push_back(typeid(VelocityComponent));
    writes.push_back(typeid(PathfindingComponent));

    reads.push_back(typeid(PositionComponent));
    reads.push_back(typeid(SizeComponent));
    reads.push_back(typeid(SpeedComponent));
}

void PathfindingSystem::update(float) {
    auto& componentManager = context.componentManager;

    auto* pathfinders = componentManager.getArray<PathfindingComponent>();
    const auto* positions = componentManager.getArray<PositionComponent>();
    const auto* sizes = componentManager.getArray<SizeComponent>();
    const auto* velocities = componentManager.getArray<VelocityComponent>();
    const auto* speeds = componentManager.getArray<SpeedComponent>();

    const float snapRadius = 3.0f;
    const float arrivalStart = 30.0f;
    const float minSpeed = 100.0f;
    const int lookAhead = 1;

    for (Entity entity : pathfinders->getEntities()) {
        const auto* pos = positions->get(entity);
        const auto* size = sizes->get(entity);
        auto* vel = velocities->get(entity);
        auto* pathFind = pathfinders->get(entity);
        const auto* speed = speeds->get(entity);

        if (pathFind && pathFind->randomOffset.x == 0) {
            generateRandomOffset(pathFind, maxOffset);
        }

        if (pos && size && vel && speed && !pathFind->reachedGoal) {
            if (pathFind->currentIndex < 0 || pathFind->currentIndex >= (int)path.size()) continue;

            int targetIndex = std::min(pathFind->currentIndex + lookAhead, (int)path.size() - 1);
            const auto* targetPos = componentManager.getComponent<PositionComponent>(path[targetIndex].entity);
            const auto* targetSize = componentManager.getComponent<SizeComponent>(path[targetIndex].entity);

            if (targetPos && targetSize) {
                const float ex = pos->x + size->w * 0.5f;
                const float ey = pos->y + size->h * 0.5f;
                const float tx = targetPos->x + targetSize->w * 0.5f + pathFind->randomOffset.x;
                const float ty = targetPos->y + targetSize->h * 0.5f + pathFind->randomOffset.y;

                float dx = tx - ex;
                float dy = ty - ey;
                const float dist = std::sqrt(dx * dx + dy * dy);

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

                if (!isLastTile) {
                    vel->x = dx * speedVal;
                    vel->y = dy * speedVal;
                } else if (isLastTile && dist <= snapRadius) {
                    pathFind->reachedGoal = true;

                    componentManager.removeComponent<PathfindingComponent>(entity);

                    DeathComponent deathComp;
                    deathComp.hasDied = true;
                    deathComp.remainingTime = 0.3f;
                    componentManager.addComponent(entity, deathComp);
                }
            }
        }
    }
}

void PathfindingSystem::generateRandomOffset(PathfindingComponent* pf, const float maxOffset) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(-maxOffset, maxOffset);
    pf->randomOffset.x = dist(gen);
    pf->randomOffset.y = dist(gen);
}
