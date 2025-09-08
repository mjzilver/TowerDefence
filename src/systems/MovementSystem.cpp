#include "MovementSystem.h"

#include "../components/DirectionComponent.h"
#include "../components/PositionComponent.h"
#include "../components/VelocityComponent.h"
#include "../components/TextureComponent.h"

void MovementSystem::update(float deltaTime) {
    for (Entity entity : getEntities()) {
        auto* position = componentManager.getComponent<PositionComponent>(entity);
        auto* velocity = componentManager.getComponent<VelocityComponent>(entity);
        auto* direction = componentManager.getComponent<DirectionComponent>(entity);
        auto* texture = componentManager.getComponent<TextureComponent>(entity);

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