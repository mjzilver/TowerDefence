#include "MovementSystem.h"

#include "../components/DirectionComponent.h"
#include "../components/PositionComponent.h"
#include "../components/TextureComponent.h"
#include "../components/VelocityComponent.h"

void MovementSystem::update(float deltaTime) {
    auto* positions = componentManager.getArray<PositionComponent>();
    auto* velocities = componentManager.getArray<VelocityComponent>();
    auto* directions = componentManager.getArray<DirectionComponent>();
    auto* textures = componentManager.getArray<TextureComponent>();

    for (Entity entity : velocities->getEntities()) {
        auto* position = positions->get(entity);
        auto* velocity = velocities->get(entity);
        auto* direction = directions->get(entity);
        auto* texture = textures->get(entity);

        if (position && velocity) {
            if (velocity->x == 0 && velocity->y == 0) {
                continue;
            }

            position->x += velocity->x * deltaTime;
            position->y += velocity->y * deltaTime;

            if (direction) {
                if (std::abs(velocity->x) > std::abs(velocity->y)) {
                    direction->direction = (velocity->x > 0) ? Direction::EAST : Direction::WEST;
                } else {
                    direction->direction = (velocity->y > 0) ? Direction::SOUTH : Direction::NORTH;
                }

                // if direction is west, flip the texture
                if (texture) {
                    texture->flipped = direction->direction == Direction::WEST;
                }
            }
        }
    }
}