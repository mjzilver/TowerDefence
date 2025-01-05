#include "MovementSystem.h"

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
                    direction->direction = (velocity->x > 0) ? Direction::East : Direction::West;
                } else {
                    direction->direction = (velocity->y > 0) ? Direction::South : Direction::North;
                }

                // if direction is west, flip the texture
                if (texture) {
                    texture->flipped = direction->direction == Direction::West;
                }
            }
        }
    }
}