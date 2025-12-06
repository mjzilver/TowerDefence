#include "PathfindingSystem.h"

#include <random>

#include "../components/FlagComponent.h"
#include "../components/PathfindingComponent.h"
#include "../components/PositionComponent.h"
#include "../components/SizeComponent.h"
#include "../components/SpeedComponent.h"
#include "../components/VelocityComponent.h"
#include "../ecs/Component.h"
#include "../utils/Globals.h"

void PathfindingSystem::update(float) {
    auto* pathfinders = componentManager.getArray<PathfindingComponent>();
    auto* positions = componentManager.getArray<PositionComponent>();
    auto* sizes = componentManager.getArray<SizeComponent>();
    auto* velocities = componentManager.getArray<VelocityComponent>();
    auto* speeds = componentManager.getArray<SpeedComponent>();

    for (Entity entity : pathfinders->getEntities()) {
        auto* position = positions->get(entity);
        auto* size = sizes->get(entity);
        auto* velocity = velocities->get(entity);
        auto* pathfinding = pathfinders->get(entity);
        auto* speed = speeds->get(entity);

        if (position && velocity && size && speed && !pathfinding->reachedGoal) {
            // Check if the entity is following a valid path
            if (pathfinding->currentIndex >= 0 && pathfinding->currentIndex < static_cast<int>(pathTiles.size())) {
                const auto* const TARGET_TILE = componentManager.getComponent<PositionComponent>(pathTiles[pathfinding->currentIndex]);
                const auto* const TARGET_SIZE = componentManager.getComponent<SizeComponent>(pathTiles[pathfinding->currentIndex]);

                if (TARGET_TILE && TARGET_SIZE) {
                    // Get the center positions of the entity and the target tile
                    float entityCenterX = position->x + size->w / 2.0f;
                    float entityCenterY = position->y + size->h / 2.0f;
                    float targetCenterX = TARGET_TILE->x + TARGET_SIZE->w / 2.0f + pathfinding->randomOffset.x;
                    float targetCenterY = TARGET_TILE->y + TARGET_SIZE->h / 2.0f + pathfinding->randomOffset.y;

                    // Calculate direction to the target
                    float dx = targetCenterX - entityCenterX;
                    float dy = targetCenterY - entityCenterY;
                    float distance = std::sqrt(dx * dx + dy * dy);

                    const float ARRIVAL_START = 30.0f;
                    const float SNAP_RADIUS = 3.0f;
                    const float MIN_SPEED = 80.0f;

                    if (distance > SNAP_RADIUS) {
                        dx /= distance;
                        dy /= distance;

                        float desiredSpeed = speed->speed;

                        // Begin soft braking inside arrivalStart
                        if (distance < ARRIVAL_START) {
                            float t = distance / ARRIVAL_START;
                            t = t * t;  // Smoothing
                            desiredSpeed = MIN_SPEED + (speed->speed - MIN_SPEED) * t;
                        }

                        velocity->x = dx * desiredSpeed;
                        velocity->y = dy * desiredSpeed;
                    } else {
                        pathfinding->currentIndex++;

                        if (pathfinding->currentIndex >= (int)pathTiles.size()) {
                            pathfinding->reachedGoal = true;

                            componentManager.scheduleDestruction(entity);
                        } else {
                            auto* nextTileSize = componentManager.getComponent<SizeComponent>(pathTiles[pathfinding->currentIndex]);
                            if (nextTileSize) {
                                generateRandomOffset(pathfinding, nextTileSize);
                            }
                        }
                    }
                }
            }
        }
    }
}

void PathfindingSystem::generatePath() {
    const auto& waypoints = mapLoader.waypoints;

    for (const auto& waypoint : waypoints) {
        Entity matchingTile = INVALID_ENTITY;

        for (Entity pathTile : entityManager->getEntities()) {
            auto* position = componentManager.getComponent<PositionComponent>(pathTile);
            auto* size = componentManager.getComponent<SizeComponent>(pathTile);
            auto* flag = componentManager.getComponent<FlagComponent>(pathTile);

            if (position && size && flag && (flag->type == FlagType::PATH || flag->type == FlagType::START || flag->type == FlagType::END)) {
                if (flag->type == FlagType::START) {
                    start = pathTile;
                } else if (flag->type == FlagType::END) {
                    end = pathTile;
                }

                if (position->x == waypoint.x * TILE_SIZE && position->y == waypoint.y * TILE_SIZE) {
                    matchingTile = pathTile;
                    break;
                }
            }
        }

        pathTiles.push_back(matchingTile);
    }
}

void PathfindingSystem::generateRandomOffset(PathfindingComponent* pathfinding, SizeComponent* targetSize) {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    float maxOffsetX = targetSize->w * 0.25f;
    float maxOffsetY = targetSize->h * 0.25f;

    std::uniform_real_distribution<float> distX(-maxOffsetX, maxOffsetX);
    std::uniform_real_distribution<float> distY(-maxOffsetY, maxOffsetY);

    pathfinding->randomOffset.x = distX(gen);
    pathfinding->randomOffset.y = distY(gen);
}