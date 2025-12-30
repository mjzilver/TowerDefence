#include "MovementSystem.h"

#include "../components/DirectionComponent.h"
#include "../components/PositionComponent.h"
#include "../components/TextureComponent.h"
#include "../components/VelocityComponent.h"

MovementSystem::MovementSystem(EngineContext& ctx) : System(ctx, "MovementSystem") {
    writes.push_back(typeid(PositionComponent));
    writes.push_back(typeid(DirectionComponent));
    writes.push_back(typeid(TextureComponent));

    reads.push_back(typeid(VelocityComponent));
}

void MovementSystem::update(float deltaTime) {
    auto& componentManager = context.componentManager;
    auto* positions = writeArray<PositionComponent>();
    const auto* velocities = readArray<VelocityComponent>();
    auto* directions = writeArray<DirectionComponent>();
    auto* textures = writeArray<TextureComponent>();

    for (Entity entity : velocities->getEntities()) {
        auto* position = positions->get(entity);
        const auto* velocity = velocities->get(entity);
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
