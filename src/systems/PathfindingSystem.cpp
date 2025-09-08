#include "PathfindingSystem.h"

#include "../components/FlagComponent.h"
#include "../components/PathfindingComponent.h"
#include "../components/PositionComponent.h"
#include "../components/SizeComponent.h"
#include "../components/SpeedComponent.h"
#include "../components/VelocityComponent.h"
#include "../ecs/Component.h"
#include "../utils/Globals.h"

void PathfindingSystem::update(float) {
    for (Entity entity : getEntities()) {
        auto* position = componentManager.getComponent<PositionComponent>(entity);
        auto* size = componentManager.getComponent<SizeComponent>(entity);
        auto* velocity = componentManager.getComponent<VelocityComponent>(entity);
        auto* pathfinding = componentManager.getComponent<PathfindingComponent>(entity);
        auto* speed = componentManager.getComponent<SpeedComponent>(entity);

        if (position && velocity && pathfinding && size && speed) {
            // Check if the entity is following a valid path
            if (pathfinding->currentIndex >= 0 && pathfinding->currentIndex < static_cast<int>(pathTiles.size())) {
                const auto* const TARGET_TILE = componentManager.getComponent<PositionComponent>(pathTiles[pathfinding->currentIndex]);
                const auto* const TARGET_SIZE = componentManager.getComponent<SizeComponent>(pathTiles[pathfinding->currentIndex]);

                if (TARGET_TILE && TARGET_SIZE) {
                    // Get the center positions of the entity and the target tile
                    float entityCenterX = position->x + size->w / 2.0f;
                    float entityCenterY = position->y + size->h / 2.0f;
                    float targetCenterX = TARGET_TILE->x + TARGET_SIZE->w / 2.0f;
                    float targetCenterY = TARGET_TILE->y + TARGET_SIZE->h / 2.0f;

                    // Calculate direction to the target
                    float dx = targetCenterX - entityCenterX;
                    float dy = targetCenterY - entityCenterY;
                    float distance = std::sqrt(dx * dx + dy * dy);

                    if (distance > 1.0f) {
                        dx /= distance;
                        dy /= distance;
                        velocity->x = dx * speed->speed;
                        velocity->y = dy * speed->speed;
                    } else {
                        if (pathTiles[pathfinding->currentIndex] == end) {
                            componentManager.removeComponent<PathfindingComponent>(entity);
                            velocity->x = 0;
                            velocity->y = 0;
                        }
                        pathfinding->currentIndex++;
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
