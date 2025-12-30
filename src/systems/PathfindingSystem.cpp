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

void PathfindingSystem::update(float deltaTime) {
    auto& componentManager = context.componentManager;

    auto* pathfinders = componentManager.getArray<PathfindingComponent>();
    const auto* positions = componentManager.getArray<PositionComponent>();
    const auto* sizes = componentManager.getArray<SizeComponent>();
    const auto* velocities = componentManager.getArray<VelocityComponent>();
    const auto* speeds = componentManager.getArray<SpeedComponent>();

    for (Entity entity : pathfinders->getEntities()) {
        const auto* pos = positions->get(entity);
        const auto* size = sizes->get(entity);
        auto* vel = velocities->get(entity);
        auto* pathFind = pathfinders->get(entity);
        const auto* speed = speeds->get(entity);

        if (pos && size && vel && speed && pathFind && !pathFind->reachedGoal) {
            if (pathFind->randomOffset.x == 0) {
                generateRandomOffset(pathFind, maxOffset);
            }

            if (pathFind->currentIndex < 0 || pathFind->currentIndex >= (int)path.size()) continue;

            int targetIndex = pathFind->currentIndex;
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

                if (dist > 0.001f) {
                    dx /= dist;
                    dy /= dist;
                }

                float moveDist = speed->speed * deltaTime;
                float stepDist = moveDist / targetCheckSteps;
                bool reached = false;
                float checkX = ex;
                float checkY = ey;

                for (int i = 0; i < targetCheckSteps; ++i) {
                    checkX += dx * stepDist;
                    checkY += dy * stepDist;

                    float stepToTarget = std::sqrt((tx - checkX) * (tx - checkX) + (ty - checkY) * (ty - checkY));
                    if (stepToTarget <= targetReachThreshold) {
                        reached = true;
                        break;
                    }
                }

                if (reached) {
                    pathFind->currentIndex++;
                    bool isLastTile = (pathFind->currentIndex >= (int)path.size());

                    if (isLastTile) {
                        pathFind->reachedGoal = true;
                        componentManager.removeComponent<PathfindingComponent>(entity);

                        DeathComponent deathComp;
                        deathComp.hasDied = true;
                        deathComp.remainingTime = 0.3f;
                        componentManager.addComponent(entity, deathComp);
                    }
                }

                if (!pathFind->reachedGoal) {
                    vel->x = dx * speed->speed;
                    vel->y = dy * speed->speed;
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
