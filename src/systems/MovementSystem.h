#pragma once
#include "../components/DirectionComponent.h"
#include "../components/PositionComponent.h"
#include "../components/VelocityComponent.h"
#include "../components/TextureComponent.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/System.h"

class MovementSystem : public System {
public:
    MovementSystem(ComponentManager& componentManager) : componentManager(componentManager) {}

    void update(float deltaTime) override {
        for (Entity entity : entities) {
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

private:
    ComponentManager& componentManager;
};